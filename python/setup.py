"""
Setup script for OmNomNum Python wrapper.
"""

from setuptools import setup, find_packages
import os

# Read README
readme_path = os.path.join(os.path.dirname(__file__), "README.md")
if os.path.exists(readme_path):
    with open(readme_path, encoding="utf-8") as f:
        long_description = f.read()
else:
    long_description = "Python wrapper for OmNomNum - normalizes numbers in text"

setup(
    name="omnomnum",
    version="0.1.0",
    author="Jesse Buesking",
    author_email="jessebuesking@gmail.com",
    description="Python wrapper for OmNomNum - normalizes numbers in text",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/JesseBuesking/omnomnum",
    packages=find_packages(),
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Topic :: Text Processing",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "License :: OSI Approved :: BSD License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: C",
    ],
    python_requires=">=3.7",
    install_requires=[
        "cffi>=1.15.0",
    ],
    setup_requires=[
        "cffi>=1.15.0",
    ],
    cffi_modules=["omnomnum/build_cffi.py:ffibuilder"],
    extras_require={
        "dev": [
            "pytest>=7.0.0",
            "pytest-cov>=4.0.0",
            "black>=22.0.0",
            "mypy>=0.990",
        ],
    },
    keywords="text-processing number-parsing nlp normalization",
    project_urls={
        "Bug Reports": "https://github.com/JesseBuesking/omnomnum/issues",
        "Source": "https://github.com/JesseBuesking/omnomnum",
    },
)
