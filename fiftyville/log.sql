-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Find what happened that day
SELECT description FROM crime_scene_reports
WHERE year = 2023 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
-- CS50 duck was stolen at 10:15 at the bakery, all interviews mention bakery.


-- Get interviews from that day
SELECT transcript FROM interviews
WHERE year = 2023 AND month = 7 AND day = 28 AND transcript LIKE '%bakery%';
-- Thief left in car within 10 minutes of robery
-- Thief withdrew money from ATM on Leggett Street before robbery. Owner of the bakery is called Emma.
-- Thief called someone and talked for less than a minute. Planning on taking earliest flight out of fiftyville the next day. Thief asked for other person to purchase plane tickets.


-- Get bakery log from that day 10 min window
SELECT hour, minute, activity, license_plate FROM bakery_security_logs
WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute > 14 AND minute < 26;
-- These license plates exited bakery in the following 10 minutes 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55


-- Find out who license paltes belong to
SELECT name FROM people
WHERE license_plate IN
(
    SELECT license_plate FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute > 14 AND minute < 26
);
-- Plates belong to Vanessa, Barry, Iman, Sofia, Luca, Diana, Kelsey, Bruce


-- See atm transactions
SELECT account_number, amount FROM atm_transactions
WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';
-- Money was withdrawn from these accounts 28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199


-- Get name associated with account
SELECT name FROM people
WHERE id IN
(
    SELECT person_id FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
    )
);
-- These people withdrew money that day Kenny, Iman, Benista, Taylor, Brooke, Luca, Diana, Bruce


-- See call logs in the followig 10 minutes
SELECT caller, receiver, duration FROM phone_calls
WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 61;
-- There where 10 calls that day that lasted 1 minute or less


-- See who the caller and receivers are
SELECT name FROM people
WHERE phone_number IN
(
    SELECT caller FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 61
);
SELECT name FROM people
WHERE phone_number IN
(
    SELECT receiver FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 61
);
-- Kenny, Sofia, Benista, Taylor, Diana, Kelsey, Kathryn, Bruce, Carina made calls
-- James, Larry, Luca, Anna, Jack, Melissa, Jacqueline, Philip, Robin, Doris received calls


-- See what plane left airport the following day first and its destination
SELECT city, full_name, abbreviation FROM airports
WHERE id IN
(
    SELECT destination_airport_id FROM flights
    WHERE year = 2023 AND month = 7 AND day = 29 AND origin_airport_id IN
    (
        SELECT id FROM airports
        WHERE city = 'Fiftyville'
    )
    ORDER BY hour ASC, minute ASC
    LIMIT 1
);
-- First plane went to New York City


-- See who was on that first plane
SELECT name FROM people
WHERE passport_number IN
(
    SELECT passport_number FROM passengers
    WHERE flight_id IN
    (
        SELECT id FROM flights
        WHERE year = 2023 AND month = 7 AND day = 29 AND origin_airport_id IN
        (
            SELECT id FROM airports
            WHERE city = 'Fiftyville'
        )
        AND destination_airport_id IN
        (
            SELECT id FROM airports
            WHERE city = 'New York City'
        )

    )
);
-- People on the plane are Kenny, Sofia, Taylor, Luca, Kelsey, Edward, Bruce, Doris
-- Thief is Bruce


-- See who Bruce called that day
SELECT name FROM people
WHERE phone_number IN
(
    SELECT receiver FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND caller IN
    (
        SELECT phone_number FROM people
        WHERE name = 'Bruce'
    )
);
-- Bruce called Gregory, Carl, Robin, Deborah
-- Accomplice is Robin



-- Queries for final solution
-- For THIEF
SELECT name FROM people
WHERE license_plate IN
(
    SELECT license_plate FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute > 14 AND minute < 26
)
AND id IN
(
    SELECT person_id FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
    )
)
AND phone_number IN
(
    SELECT caller FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 61
)
AND passport_number IN
(
    SELECT passport_number FROM passengers
    WHERE flight_id IN
    (
        SELECT id FROM flights
        WHERE year = 2023 AND month = 7 AND day = 29 AND origin_airport_id IN
        (
            SELECT id FROM airports
            WHERE city = 'Fiftyville'
        )
        AND destination_airport_id IN
        (
            SELECT id FROM airports
            WHERE city = 'New York City'
        )

    )
);


-- FOR ESCAPE CITY
SELECT city FROM airports
WHERE id IN
(
    SELECT destination_airport_id FROM flights
    WHERE year = 2023 AND month = 7 AND day = 29 AND origin_airport_id IN
    (
        SELECT id FROM airports
        WHERE city = 'Fiftyville'
    )
    ORDER BY hour ASC, minute ASC
    LIMIT 1
);


-- For ACCOMPLICE
SELECT name FROM people
WHERE phone_number IN
(
    SELECT receiver FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND caller IN
    (
        SELECT phone_number FROM people
        WHERE name = 'Bruce'
    )
)
AND phone_number IN
(
    SELECT receiver FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 61
);
