import click
import subprocess
import time


@click.command()
@click.option('-n', '--number', default=10)
@click.argument('command', nargs=-1, type=click.UNPROCESSED)
def benchmark(number, command):
    timings = []

    print('command = {}'.format(' '.join(command)))
    for i in range(number):
        start = time.time()
        subprocess.call(command)
        timing = time.time() - start
        timings.append(timing)
        print(f'{i+1:>4} took {timing:.3f}s')

    avg = sum(timings) / len(timings)
    med = sorted(timings)[len(timings) >> 1]

    print(f'avg = {avg:.3f}s')
    print(f'med = {med:.3f}s')


if __name__ == '__main__':
    benchmark()
