import click
import math
import os
from colorama import init, Fore, Back, Style

_INITIAL_DIVIDERS = (
    (3, 9),
    (5, 25),
    (7, 49),
    (11, 121),
    (13, 169),
    (17, 289),
    (19, 361),
    (23, 529),
    (29, 841),
    (31, 961),
    (37, 1369),
    (41, 1681),
    (43, 1849),
    (47, 2209),
    (53, 2809),
    (59, 3481),
    (61, 3721),
)

def is_prime(n):
    if n & 1 == 0:
        return n == 2
    if n == 3:
        return True
    if n < 2:
        return False

    for d, dxd in _INITIAL_DIVIDERS:
        if dxd > n:
            return True
        if n % d == 0:
            return False

    max_d = int(math.sqrt(n) + 1)
    for d in range(65, max_d, 6):
        if n % d == 0:
            return False
        if n % (d + 2) == 0:
            return False

    return True

def digits_sum(i):
    return sum(int(c) for c in str(i))

def reduce_i(i):
    while i > 9:
        i = digits_sum(i)
    return i

def reduce_i_fast(i):
    if i == 0:
        return 0
    m = i % 9
    return 9 if m == 0 else m

def reduce_div(i):
    """Returns reduce_i if the number can be divided by it"""
    NOT = ' '
    if i == 0:
        return NOT
    ri = reduce_i(i)
    if i % ri == 0:
        return ri
    return NOT

PRIME = '█'
def reduce_div_or_prime(i):
    div = reduce_div(i)
    if is_prime(i):
        return PRIME
    return div

def chunkify(sliceable, size):
    return ((i, sliceable[i:i+size]) for i in range(0, len(sliceable), size))

def get_terminal_size():
    rows, cols = os.popen('stty size', 'r').read().split()
    return int(rows), int(cols)

algo_mapping = {
    'reduce': reduce_i,
    'reducediv': reduce_div,
    'reducedivp': reduce_div_or_prime,
}

color_mapping = {
    '▌': Fore.BLACK + Style.DIM,
    '▐': Fore.BLACK + Style.DIM,
    PRIME: Fore.WHITE + Style.BRIGHT,
    '0': Fore.WHITE,
    '8': Fore.RED,
    '7': Fore.RED + Style.BRIGHT,
    '6': Fore.CYAN,
    '5': Fore.CYAN + Style.BRIGHT,
    '4': Fore.MAGENTA,
    '3': Fore.MAGENTA + Style.BRIGHT,
    '2': Fore.WHITE,
    '1': Fore.WHITE,
    '9': Fore.WHITE,
}
COLOR_RESET = Fore.RESET + Back.RESET + Style.RESET_ALL

def colorize(string):
    new_string = ''
    for c in string:
        try:
            color = color_mapping[c]
            new_string += color + c + COLOR_RESET
        except KeyError:
            new_string += c
    return new_string

@click.group()
def cli():
    pass

@cli.command()
@click.option('-s', '--start', type=int, default=0)
@click.option('-e', '--end', type=int, default=None)
@click.option('-E', '--length', type=int, default=None)
@click.option('-z', '--size', default=10)
@click.option('-a', '--algo', default='reduce')
def run(start, end, length, size, algo):
    rows, cols = get_terminal_size()
    if end is None:
        if length is not None:
            end = start + length
        else:
            end = start + (size * (rows - 2))
    fn = algo_mapping[algo]
    results = [fn(i) for i in range(start, end)]
    for s, l in chunkify(results, size):
        output_line = colorize('{:>10} ▌{}▐ '.format(
            s+start,
            ''.join([str(i) for i in l]),
        ))
        print(output_line)

@cli.command()
def test():
    for i in range(0, 1000000):
        ri = reduce_i(i)
        rif = reduce_i_fast(i)
        print(f'{i:>8} {ri} {rif}')
        assert ri == rif

if __name__ == '__main__':
    cli()
