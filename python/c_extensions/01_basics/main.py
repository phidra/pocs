def main() -> None:
    print(
        """
This POC shows a basic C extension module for python.

It is the implementation of the example given in this doc :

https://docs.python.org/3/extending/extending.html
"""
    )

    print("")
    print("BEFORE using 'pouet' module")
    import pouet  # noqa: F401

    pouet.system("ls -lh")
    print("AFTER  using 'pouet' module")
    print("")


if __name__ == "__main__":
    main()
