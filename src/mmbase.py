import re

class _:
    def __init__(self, *args, **kargs):
        self._ = (args, kargs)
    def __call__(self, *args):
        return self._ + (args, )

class Primitive:
    def __init__(self, name, min, max, include=None):
        self.name = name
        self.min = min
        self.max = max
        self.include = include
    def __str__(self):
        return self.name

primitives = [
    _('bool', 'false', 'true')
        ('bool'),
    _('char', '((char)-128)', '((char)127)')
        ('char', 'int8'),
    _('short', '((short)-32768)', '((short)32767)')
        ('short', 'int16'),
    _('int', '-0x80000000', '0x7FFFFFFF')
        ('int', 'int32'),
    _('long long', '-0x8000000000000000ll', '0x7FFFFFFFFFFFFFFFll')
        ('long', 'int64'),
    _('unsigned char', '((unsigned char)0)', '((unsigned char)255)')
        ('uchar', 'uint8'),
    _('unsigned short', '((unsigned short)0)', '((unsigned short)65535)')
        ('ushort', 'uint16'),
    _('unsigned int', '0u', '4294967295u')
        ('uint', 'uint32'),
    _('unsigned long long', '0ull', '0xFFFFFFFFFFFFFFFFull')
        ('ulong', 'uint64'),
    _('float', '(-FLT_MAX)', 'FLT_MAX', include='cfloat')
        ('float32'),
    _('double', '(-DBL_MAX)', 'DBL_MAX', include='cfloat')
        ('float', 'double', 'float64'),
    _('size_t', '((size_t)0)', '((size_t)0xFFFFFFFFFFFFFFFFull)', include='cstddef')
        ('size_t'),
]

class Function:
    def __init__(self, name, ret_type=None):
        self.name = name
        self.ret_type = ret_type
    def init2(self, symbols):
        if isinstance(self.ret_type, str) and self.ret_type in symbols:
            self.ret_type = symbols[self.ret_type]
    def __str__(self):
        return self.name

functions = [
    _('inputInt', 'int')
        ('inputInt'),
    _('inputWord', 'string')
        ('inputWord'),
    _('print', None)
        ('print'),
]

class Method:
    def __init__(self, name, ret_type=None):
        self.name = name
        self.ret_type = ret_type
    def init2(self, symbols):
        if isinstance(self.ret_type, str) and self.ret_type in symbols:
            self.ret_type = symbols[self.ret_type]
    def __str__(self):
        return self.name

class Class:
    def __init__(self, name, member=[], methods=[]):
        self.name = name
        self.methods = {}
        for args, kargs, names in methods:
            method = Method(*args, **kargs)
            for name in names:
                self.methods[name] = method
    def init2(self, symbols):
        for name in self.methods:
            self.methods[name].init2(symbols)
    def __str__(self):
        return self.name

classes = [
]

class TemplateClass:
    def __init__(self, name, member=[], methods=[]):
        self.name = name
        self.methods = {}
        for args, kargs, names in methods:
            method = Method(*args, **kargs)
            for name in names:
                self.methods[name] = method

    def init2(self, symbols):
        for name in self.methods:
            self.methods[name].init2(symbols)
    def __str__(self):
        return self.name
    def __getitem__(self, name):
        return self.methods[name]

template_classes = [
    _('std::basic_string', methods=[
        _('resize', )('resize'),
        _('size', 'size_t')('size'),
        _('substr', '$self')('substr'),
        _('at', '$1')('at'),
        _('starts_with', 'bool')('starts_with'),
        _('ends_with', 'bool')('ends_with'),
    ])('basic_string'),
]

class TemplateInstance:
    def __init__(self, template_class, *args):
        self.template_class = template_class
        self.args = args
    def init2(self, symbols):
        if isinstance(self.template_class, str) and self.template_class in symbols:
            self.template_class = symbols[self.template_class]
    def __getitem__(self, name):
        method = self.template_class[name]
        if isinstance(method.ret_type, str) and method.ret_type.startswith('$'):
            if method.ret_type=='$self':
                return Method(method.name, self)
            else:
                m = re.match(r'\$(\d+)')
                if m:
                    index = int(m.group(1))
                    assert index<len(self.args)
                    return Method(method.name, self.args[index])
        return method

template_instances = [
    _('basic_string', 'char')('string', 'str'),
]

class Value:
    def __init__(self, ty, expr):
        self.type = ty
        self.expr = expr

class Variable:
    def __init__(self, name, ty):
        self.name = name
        self.type = ty

def initSymbols():
    symbols = {}
    cpp_symbols = set()
    for args, kargs, names in primitives:
        o = Primitive(*args, **kargs)
        for name in names:
            symbols[name] = o
        cpp_symbols.add(o.name)
    for args, kargs, names in functions:
        o = Function(*args, **kargs)
        for name in names:
            symbols[name] = o
        cpp_symbols.add(o.name)
    for args, kargs, names in classes:
        o = Class(*args, **kargs)
        for name in names:
            symbols[name] = o
        cpp_symbols.add(o.name)
    for args, kargs, names in template_classes:
        o = TemplateClass(*args, **kargs)
        for name in names:
            symbols[name] = o
        cpp_symbols.add(o.name)
    for args, kargs, names in template_instances:
        o = TemplateInstance(*args, **kargs)
        for name in names:
            symbols[name] = o
    for symbol in list(symbols.values()):
        if hasattr(symbol, 'init2'):
            symbol.init2(symbols)
    return symbols, cpp_symbols
