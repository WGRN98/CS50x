import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    # Get needed databases
    portfolio_db = db.execute("SELECT symbol, shares FROM portfolio WHERE user_id = ?", user_id)
    cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cash_db[0]["cash"]

    portfolio = {}
    stock_total = 0

    # Get portfolio
    for n in portfolio_db:
        symbol, shares = n["symbol"], n["shares"]
        portfolio[symbol] = portfolio.setdefault(symbol, 0) + shares

    # Get stock values and total values, add to portfolio variable
    for symbol, shares in portfolio.items():
        price = lookup(symbol)["price"]
        stock_value = shares * price
        stock_total += stock_value
        portfolio[symbol] = (shares, usd(price), usd(stock_value))

    total = stock_total + cash

    return render_template("index.html", portfolio=portfolio, cash=usd(cash), stock_total=usd(stock_total), total=usd(total), usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        # Get user inputs from the form
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        # Validate inputs
        if not symbol:
            return apology("Must give Symbol")
        elif not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("Shares must be a positive integer")

        # Lookup stock information
        stock = lookup(symbol)
        if stock is None:
            return apology("Symbol does not exist")

        # Calculate total transaction value
        transaction_value = int(shares) * stock["price"]

        # Get user information
        user_id = session["user_id"]
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        user_cash = user_cash_db[0]["cash"]

        # Check if the user can afford the transaction
        if user_cash < transaction_value:
            return apology("Not enough money")

        # Update user cash
        update_cash = user_cash - transaction_value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", update_cash, user_id)

        # Check if user already has stock, update portfolio accordingly
        instock = db.execute(
            "SELECT symbol FROM portfolio WHERE user_id = ? and symbol = ?", user_id, symbol)
        if not instock:
            db.execute("INSERT INTO portfolio (user_id, symbol, shares) VALUES(?, ?, ?)",
                       user_id, symbol, shares)

        else:
            current_stock = db.execute(
                "SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?", user_id, symbol)
            db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol =?",
                       int(current_stock[0]["shares"]) + int(shares), user_id, symbol)

        # Update transactions history
        date = datetime.datetime.now()
        db.execute(
            "INSERT INTO transactions (user_id, type, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?, ?)",
            user_id, "BUY", stock["symbol"], shares, stock["price"], date)

        flash(
            f"Bought {shares} shares of {symbol} for {usd(transaction_value)}, Updated cash: {usd(update_cash)}")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Query the required info for the history page from the transactions database
    transaction_info = db.execute(
        "SELECT type, symbol, price, shares, date FROM transactions WHERE user_id = ? ORDER BY date DESC", session["user_id"])
    return render_template("history.html", transaction_info=transaction_info, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""
    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")

        # Check if symbol is valid
        if not symbol:
            return apology("Must give a symbol")

        # Lookup stock ticker and price
        stock = lookup(symbol.upper())

        # Return current price if valid symbol
        if stock == None:
            return apology("Symbol does not exist")
        return render_template("quoted.html", symbol=stock["symbol"], price=stock["price"])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("register.html")

    # User reached route via POST (as by submitting a form via POST)
    username = request.form.get("username")
    pw = request.form.get("password")
    confirm_pw = request.form.get("confirmation")

    # Username and Password Validation
    if not username:
        return apology("You should input the username")
    elif not pw:
        return apology("You should input your password")
    elif not confirm_pw:
        return apology("You should input your password in 'Confirmation Password'")
    elif pw != confirm_pw:
        return apology("Password does not match")

    # Hash the password
    hashed_pw = generate_password_hash(pw)

    # Insert username and hashed password into the database
    try:
        db.execute("INSERT INTO users(username, hash) VALUES(?, ?)", username, hashed_pw)
    # If the username already exist in the database
    except:
        return apology("Username registered by others already")

    # Redirect the user to login form
    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        user_id = session["user_id"]
        symbols_user = db.execute(
            "SELECT symbol FROM portfolio WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", user_id)
        return render_template("sell.html", symbols=[row["symbol"] for row in symbols_user])
    else:
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Validate inputs
        if not symbol:
            return apology("Must insert a symbol")
        # Lookup stock information
        stock = lookup(symbol.upper())
        if stock == None:
            return apology("Symbol does not exist")

        if shares < 0:
            return apology("Share number not allowed")

        # Total transaction value
        transaction_value = shares * stock["price"]

        # Get user available cash
        user_id = session["user_id"]
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        user_cash = user_cash_db[0]["cash"]

        # Get user shares
        user_shares_db = db.execute(
            "SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, symbol)
        user_shares = user_shares_db[0]["shares"]

        # See if user has enough shares for sale
        if shares > user_shares:
            return apology("You do not have enough shares of this stock")

        # Update user cash
        updt_cash = user_cash + transaction_value
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updt_cash, user_id)

        # Update user portfolio
        updt_shares = int(user_shares - shares)
        if updt_shares == 0:
            db.execute("DELETE FROM portfolio WHERE symbol = ?", symbol)
        else:
            db.execute("UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",
                       updt_shares, user_id, symbol)

        # Update user history
        date = datetime.datetime.now()
        db.execute("INSERT INTO transactions (user_id, type, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?, ?)",
                   user_id, "SELL", stock["symbol"], (-1)*shares, stock["price"], date)

        flash(
            f"Sold {shares} shares of {symbol} for {usd(transaction_value)}, Updated cash: {usd(updt_cash)}")

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("add_cash.html")
    else:
        amount = int(request.form.get("amount"))

        # Check if amount is valid
        if int(amount) <= 0:
            return apology("Invalid amount!")

        # Get user information
        user_id = session["user_id"]
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        user_cash = user_cash_db[0]["cash"]

        # Update user cash
        updt_cash = user_cash + amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updt_cash, user_id)

        flash(f"Added {usd(amount)}, Updated cash: {usd(updt_cash)}")

        return redirect("/")
