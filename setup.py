import os
import setuptools

setuptools.setup(
    name='mmlang',
    version=open(os.path.join(os.path.dirname(__file__), 'version.txt')).read(),
    packages=[
        'mmlang',
    ],
    package_dir={
        'mmlang': 'src',
    },
    package_data={
        'mmlang': ['mmlang.lark', '../version.txt', '../lib/*'],
    },
    include_package_data=True,
    install_requires=[
        'psutil',
        'lark-parser',
        'colorama',
        'atcoder-tools',
        'markupsafe==2.0.1',
    ],
    entry_points={
        'console_scripts': [
            'mm = mmlang:main',
        ],
    },
)
