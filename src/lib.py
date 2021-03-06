func_definition = {
    'inputWord': ('string', 'input_word.h'),
    'inputInt': ('int', 'input_int.h'),
    'inputInt64': ('long long', 'input_int64.h'),
    'inputInt128': ('__int128', 'input_int128.h'),
    'inputFraction': (('int', 'int'), 'input_fraction.h'),
    'inputFraction64': (('long long', 'int'), 'input_fraction64.h'),
    'inputFraction128': (('__int128', 'int'), 'input_fraction128.h'),
    'inputFloat': ('double', 'input_float.h'),
    'print': (None, 'print.h'),
    'print0': (None, 'print.h'),
    'mod': ('int', 'mod.h'),
    'mod_mul': ('int', 'mod_mul.h'),
    'int': ('int', None),
    'char': ('char', None),
    'int64': ('long long', None),
    'int128': ('__int128', None),
    'float': ('double', None),
    'string': ('string', 'string'),
    'container_pop': (None, 'container_pop.h'),
    'container_shift': (None, 'container_shift.h'),
    'cos': ('double', 'cmath'),
    'sin': ('double', 'cmath'),
    'sqrt': ('double', 'cmath'),
    'log2': ('double', 'cmath'),
    'exp2': ('double', 'cmath'),
    'ceil': ('double', 'cmath'),
    'abs': (None, 'cmath'),
    'BIT': (None, 'BIT.h'),
    'set': ('set', 'set'),
    'multiset': ('multiset', 'set'),
    'atoi': ('int', 'cstdlib'),
    'atoll': ('long long', 'cstdlib'),
    '_slice': ('_slice', '_slice.h'),
    'range': ('vector<int>', 'range.h'),
    'rrange': ('vector<int>', 'range.h'),
    'unionfind': ('unionfind', 'unionfind.h'),
    'gcd': ('int', 'gcd.h'),
    'gcd64': ('long long', 'gcd64.h'),
    'asm_mul_hi': ('unsigned int', 'asm_mul_hi.h'),
    'lrand49': ('int', 'lrand49.h'),
    'drand49': ('double', 'drand49.h'),
    'mrand49': ('int', 'mrand49.h'),
    'mmrand49': ('long long', 'mmrand49.h'),
    'initTime': (None, 'get_time.h'),
    'getTime': ('double', 'get_time.h'),
    'getCpuClock': (None, 'get_cpu_clock.h'),
    'getNativeTime': ('double', 'get_native_time.h'),
    'gvFps': ('bool', 'gv.h'),
    'gvTime': (None, 'gv.h'),
    'gvText': (None, 'gv.h'),
    'gvTextLeft': (None, 'gv.h'),
    'gvArrow': (None, 'gv.h'),
}
dependency = {
    'input_c_line_or_word.h': ['cassert', 'cstdio', 'cstring'],
    'input_c_word.h': ['input_c_line_or_word.h'],
    'input_word.h': ['string', 'input_c_word.h'],
    'input_int.h': ['cstdlib', 'input_c_word.h'],
    'input_int64.h': ['cstdlib', 'input_c_word.h'],
    'input_int128.h': ['input_c_word.h'],
    'input_fraction.h': ['input_c_word.h'],
    'input_fraction64.h': ['input_c_word.h'],
    'input_fraction128.h': ['input_c_word.h'],
    'input_float.h': ['cstdlib', 'input_c_word.h'],
    'print.h': ['cstdio'],
    'BIT.h': ['vector'],
    'unionfind.h': ['vector'],
    'fast_array.h': ['cassert'],
    'zobrist.h': ['cassert', 'mrand49.h', 'mmrand49.h'],
    'mmrand49.h': ['mrand49.h'],
    'fast_weak_set32.h': ['cstring', 'cassert', 'fast_array.h'],
    'fast_weak_set64.h': ['cstring', 'cassert', 'fast_array.h'],
    'get_native_time.h': ['sys/time.h'],
    'get_time.h': ['get_cpu_clock.h', 'get_native_time.h', 'cmath'],
    'dpque.h': ['vector', 'utility'],
    'xmem.h': ['cassert', 'cstring', 'unlikely.h'],
    'xarray.h': ['cassert', 'xmem.h', 'fast_array.h', 'xref.h'],
    'xvector.h': ['cassert', 'xmem.h', 'fast_array.h', 'xref.h', 'likely.h', 'unlikely.h'],
    'xbeam.h': ['get_time.h', 'fast_pool.h', 'dpque.h'],
    'lrand49.h': ['mrand49.h', 'asm_mul_hi.h'],
    'drand49.h': ['mrand49.h'],
    'mmrand49.h': ['mrand49.h'],
    'gv.h': ['cstdarg', 'cmath'],
}
nocompress = set([
])
class_definition = {
    'vector': ({
        'assign': None,
        'at': '$1',
        'back': '$1',
        'begin': 'auto',
        'capacity': 'size_t',
        'cbegin': 'auto',
        'cend': 'auto',
        'clear': None,
        'crbegin': 'auto',
        'crend': 'auto',
        'data': 'auto',
        'emplace': 'auto',
        'emplace_back': None,
        'empty': 'bool',
        'end': 'auto',
        'erase': 'auto',
        'front': '$1',
        'insert': 'auto',
        'max_size': 'size_t',
        'pop_back': None,
        'push_back': None,
        'rbegin': 'auto',
        'rend': 'auto',
        'reserve': None,
        'resize': None,
        'shrink_to_fit': None,
        'size': 'size_t',
        'swap': None,
        '__getitem__': '$1',
    }, 'vector'),
    'array': ({
        'at': '$1',
        'back': '$1',
        'begin': 'auto',
        'cbegin': 'auto',
        'cend': 'auto',
        'crbegin': 'auto',
        'crend': 'auto',
        'data': 'auto',
        'empty': 'bool',
        'end': 'auto',
        'fill': None,
        'front': '$1',
        'max_size': 'size_t',
        'rbegin': 'auto',
        'rend': 'auto',
        'size': 'size_t',
        'swap': None,
        '__getitem__': '$1',
    }, 'array'),
    'xarray': ({
        'size': 'int',
        '__getitem__': '$1',
    }, 'xarray.h'),
    'xvector': ({
        'size': 'int',
        '__getitem__': '$1',
    }, 'xvector.h'),
    'zobrist': ({
        '__getitem__': '$1',
    }, 'zobrist.h'),
    'fast_array': ({
        '__getitem__': '$1',
    }, 'fast_array.h'),
    'zobrist': ({
        '__getitem__': '$1',
    }, 'zobrist.h'),
    'fast_weak_set32': ({
    }, 'fast_weak_set32.h'),
    'fast_weak_set64': ({
    }, 'fast_weak_set64.h'),
}
