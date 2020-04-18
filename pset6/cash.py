import cs50

# verify input is positve number
changeOwedDollars = 0
while changeOwedDollars <= 0:
    changeOwedDollars = cs50.get_float("Change Owed: ")

# convert dollar change to cents change
changeOwedCents = changeOwedDollars * 100

# declare coin variables
quarter = 25
dime = 10
nickel = 5
penny = 1

coinsUsed = 0

# quarters calc
if changeOwedCents//quarter >= 1:
    quartersUsed = changeOwedCents//quarter
    changeOwedCents = changeOwedCents % quarter
    coinsUsed += quartersUsed

# dimes calc
if changeOwedCents//dime >= 1:
    dimesUsed = changeOwedCents//dime
    changeOwedCents = changeOwedCents % dime
    coinsUsed += dimesUsed

# nickels calc
if changeOwedCents//nickel >= 1:
    nickelsUsed = changeOwedCents//nickel
    changeOwedCents = changeOwedCents % nickel
    coinsUsed += nickelsUsed

# because a penny is one coin do not need to do modulo calculations
coinsUsed += changeOwedCents

print(coinsUsed)