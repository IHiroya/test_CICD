from setuptools import find_packages, setup

package_name = 'sample_py_pkg'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='IHiroya',
    maintainer_email='ikawahiroya0208@gmail.com',
    description='CI/CD学習用のPythonサンプルパッケージ(値の範囲制限ノード)',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'clamp_node = sample_py_pkg.clamp_node:main',
        ],
    },
)
