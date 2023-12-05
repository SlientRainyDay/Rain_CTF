# sage 9.2
from secrets import token_hex
from random import shuffle

p = 101
Sp = SymmetricGroup(p)
Mp = MatrixSpace(GF(p), p, p)
e = 65537

flag = open("./flag", "r").read()
flag = (flag+token_hex(p-len(flag)))[:p]


def random_perm_matrix():
    M = []
    for _ in range(p):
        L = list(range(1, p+1))
        shuffle(L)
        M.append(Sp(L))
    return M


def p2m(M):
    return Mp([m.tuple() for m in M])


def perm_power(M: list, key: str):
    m = key.encode()
    res = []
    for i, e in enumerate(m):
        res.append(M[i] ^ e)
    return res


def shuffle_plus(msg: str, shuffle_matrix: list):
    for perm in shuffle_matrix:
        msg = perm(msg)
    return msg


def matrix_encrypt(M, e):
    return M ^ e


perm_matrix = random_perm_matrix()
print(f"[+] perm_matrix = {list(p2m(perm_matrix))}")

msg = shuffle_plus(flag, perm_matrix)
M = p2m(perm_power(perm_matrix, msg))
enc = matrix_encrypt(M, e)

print(f"[+] enc = {list(enc)}")
