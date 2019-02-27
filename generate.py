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

    # Figure out the required number of statements.
    length = 1
    for d in dividers:
        length *= d

    # Mapping to figure out which macro to use, given the (possible)
    # primality of 6n-1 (left) and 6n+1 (right).
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

    # The default step count for macros.
    macro_step = {
        macro_both: 6,
        macro_left: 6,
        macro_right: 4,
        macro_none: 0,
    }

    # The first statement holds the starting position. The reason for
    # this becomes apparent later.
    statements = [{'step': start}]

    # Generate the macro statements.
    start_n = round(start / 6.0)
    for ni, n in enumerate(range(start_n, start_n + length)):
        L = 6 * n - 1
        R = 6 * n + 1
        Ldiv = _check_divider(L, dividers)
        Rdiv = _check_divider(R, dividers)
        Lprime = not bool(Ldiv)
        Rprime = not bool(Rdiv)

        macro = macro_mapping[Lprime][Rprime]
        statement = {
            'macro': macro,
            'step': macro_step[macro],
            'Ldiv': '-' if Ldiv is None else Ldiv,
            'Rdiv': '-' if Rdiv is None else Rdiv,
            'n': ni + 1,
        }
        statements.append(statement)

    # Adjust the step sizes where needed. Macros before `_6n1_right` and
    # `_6n1_none` receive a step bonus of 2 and 6 respectively.
    #
    # Because the starting position was added as a statement earlier,
    # that will be adjusted here too.
    for i in reversed(range(len(statements) - 1)):
        curr_c = statements[i]
        next_c = statements[i+1]
        if next_c['macro'] == macro_right:
            curr_c['step'] += 2
        elif next_c['macro'] == macro_none:
            #                                    Add the step size of the none macro
            #                   :--------------: which can be something other than 0.
            curr_c['step'] += 6 + next_c['step']

    # Empty names for the `_6n1_none` macro.
    empty_name_n = ' ' * len(name_n)
    empty_name_d = ' ' * len(name_d)
    empty_name_max_d = ' ' * len(name_max_d)

    # Output the code.
    print('uint64_t d = {};'.format(statements[0]['step']))
    print('while (true) {')
    for statement in statements[1:]:
        if statement['macro'] == macro_none:
            # nicely format a macro_none output
            print(template.format(
                macro=statement['macro'],
                name_n=empty_name_n,
                name_d=empty_name_d,
                name_max_d=empty_name_max_d,
                step='',
                Ldiv=statement['Ldiv'],
                Rdiv=statement['Rdiv'],
                n=statement['n'],
            ).replace(',', ' '))
        else:
            print(template.format(
                macro=statement['macro'],
                name_n=name_n,
                name_d=name_d,
                name_max_d=name_max_d,
                step=statement['step'],
                Ldiv=statement['Ldiv'],
                Rdiv=statement['Rdiv'],
                n=statement['n'],
            ))
    print('}')


@click.command()
@click.argument('start', type=int)
@click.argument('dividers', nargs=-1, type=int)
def generate(start, dividers):
    # start must be a writeable as 6n-1
    if start % 6 == 5:
        generate_6n1_code(start, dividers)
    else:
        print('%i can not be written as 6n-1. Choose another start number.' % start)


if __name__ == '__main__':
    generate()
