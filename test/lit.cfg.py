import os

import lit.formats

config.name = 'VCLLVM'
config.test_format = lit.formats.ShTest(True)

config.suffixes = ['.c', '.ll']

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.my_obj_root, 'test')

config.substitutions.append(('%VCLLVM',
                             os.path.join(config.my_obj_root, 'bin/VCLLVM')))
