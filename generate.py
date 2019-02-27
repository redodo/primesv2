import click


def _check_divider(n, dividers):
    for d in dividers:
        if n % d == 0:
            return d
    return None


def generate_6n1_code(
        start,
        dividers,
        macro_both='_6n1_both(',
        macro_left='_6n1_left(',
        macro_right='_6n1_right(',
        macro_none='_6n1_none(',
        name_n='n',
        name_d='d',
        name_max_d='max_d',
        template='    {macro:<11}{name_n:>2}, {name_d}, {name_max_d}, {step:>2}); // {Ldiv:>2} {Rdiv:>2} {n:.>4}',
        ):
    length = 1
    for d in dividers:
        length *= d

    macro_mapping = {
        # left is possibly prime
        True: {
            # right is possibly prime
            True: macro_both,
            # right is definitely not prime
            False: macro_left,
        },
        # left is definitely not prime
        False: {
            # right is possibly prime
            True: macro_right,
            # right is definitely not prime
            False: macro_none,
        },
    }
    macro_step = {
        macro_both: 6,
        macro_left: 6,
        macro_right: 4,
        macro_none: 0,
    }

    # the first command is the starting position (d = start)
    commands = [{'step': start}]
    # generate the commands
    for n in range(start, start + length):
        L = 6 * n -1
        R = 6 * n + 1
        Ldiv = _check_divider(L, dividers)
        Rdiv = _check_divider(R, dividers)
        Lprime = not bool(Ldiv)
        Rprime = not bool(Rdiv)

        macro = macro_mapping[Lprime][Rprime]
        command = {
            'macro': macro,
            'step': macro_step[macro],
            'Ldiv': '-' if Ldiv is None else Ldiv,
            'Rdiv': '-' if Rdiv is None else Rdiv,
            'n': n - start + 1,
        }
        commands.append(command)

    # fix the step size (macros before right and none receive a step bonus of 2 and 6 respectively)
    for i in range(len(commands) - 1):
        curr_c = commands[i]
        next_c = commands[i+1]
        if next_c['macro'] == macro_right:
            curr_c['step'] += 2
        elif next_c['macro'] == macro_none:
            curr_c['step'] += 6

    # empty names for none macro
    empty_name_n = ' ' * len(name_n)
    empty_name_d = ' ' * len(name_d)
    empty_name_max_d = ' ' * len(name_max_d)

    # output the code
    print('uint64_t d = {};'.format(commands[0]['step']))
    print('while (true) {')
    for command in commands[1:]:
        if command['macro'] == macro_none:
            # nicely format a macro_none output
            print(template.format(
                macro=command['macro'],
                name_n=empty_name_n,
                name_d=empty_name_d,
                name_max_d=empty_name_max_d,
                step='',
                Ldiv=command['Ldiv'],
                Rdiv=command['Rdiv'],
                n=command['n'],
            ).replace(',', ' '))
        else:
            print(template.format(
                macro=command['macro'],
                name_n=name_n,
                name_d=name_d,
                name_max_d=name_max_d,
                step=command['step'],
                Ldiv=command['Ldiv'],
                Rdiv=command['Rdiv'],
                n=command['n'],
            ))
    print('}')


@click.command()
@click.argument('start', type=int)
@click.argument('dividers', nargs=-1, type=int)
def generate(start, dividers):
    # start must be a writeable as 6n±1
    if start % 6 & 3 == 1:
        generate_6n1_code(start, dividers)
    else:
        print('Start can not be written as 6n±1. Stopped code generation.')


if __name__ == '__main__':
    generate()
