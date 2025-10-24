from setuptools import setup, find_packages

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

with open("requirements.txt", "r", encoding="utf-8") as fh:
    requirements = [line.strip() for line in fh if line.strip() and not line.startswith("#")]

setup(
    name="modbus-product",
    version="0.1.0",
    author="Nguyen Tien Dung",
    author_email="support@modbusproduct.com",
    description="A professional Python library for Modbus communication in industrial IoT applications",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/nguyentiendungIoT/modbus_product",
    packages=find_packages(),
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Manufacturing",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Topic :: System :: Hardware",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Operating System :: OS Independent",
    ],
    python_requires=">=3.7",
    install_requires=requirements,
    extras_require={
        "dev": [
            "pytest>=7.0.0",
            "pytest-cov>=4.0.0",
            "black>=23.0.0",
            "flake8>=6.0.0",
            "mypy>=1.0.0",
            "isort>=5.12.0",
        ],
    },
    keywords="modbus, industrial, iot, automation, tcp, rtu, serial, communication",
    project_urls={
        "Bug Reports": "https://github.com/nguyentiendungIoT/modbus_product/issues",
        "Source": "https://github.com/nguyentiendungIoT/modbus_product",
        "Documentation": "https://github.com/nguyentiendungIoT/modbus_product/tree/main/docs",
    },
)
