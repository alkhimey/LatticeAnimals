results = [1,
           3,
           15,
           86,
           498,
           2797,
           14988,
           76216,
           368040,
           1694142,
           7471574,
           31736892,
           130483178]


"""
results = [1,
           3,
           15,
           86,
           522,
           3241,
           20256,
           126520,
           787224,
           4873750,
           30019640]
"""


def prime_factors(x):
    res = []
    n = 2;
    while x >= n:
        if (x % n == 0):
            res += [n]
            x = x / n
        else:
            n = n + 1
    return res
"""
div_seq = [results[i] / float(results[i-1]) for i in range(1, len(results))]
print "Div ssequence: ", div_seq


print [div_seq[i] - float(div_seq[i-1]) for i in range(1, len(div_seq))]


print "Prime factors:"
for x in results:
    print x, "      \t", prime_factors(x)


for k in range(1, 6):
    for m in range (1, 10):
        z =  [x - m*4**(i-k) for (i,x) in enumerate(results)][k:]
        count = 0
        for x in z:
            count += len(prime_factors(x))
        if count > len(z) * 4:
            print z, k, m
"""

k = 4
m = 9
z =  [x - m*4**(i-k) for (i,x) in enumerate(results)][k:]
print "Prime factors:"
for x in z:
    print x, "      \t", prime_factors(x)
