text = input("Text: ")

letters = sum(1 for c in text if c.isalpha())
words = len(text.split())
sentences = text.count('.') + text.count('!') + text.count('?')

L = letters / words * 100
S = sentences / words * 100

index = round(0.0588 * L - 0.296 * S - 15.8)

if index < 1:
    print("Before Grade 1")

elif index >= 1 and index < 16:
    print("Grade", int(index))

else:
    print("Grade 16+")
