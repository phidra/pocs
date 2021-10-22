#!/usr/bin/env python3

from typing import Union, TypeVar

MyType = TypeVar("MyType", str, bytes)  # a MyType instance is either a str or a bytes


def concatenate_with_union(x: Union[str, bytes], y: Union[str, bytes]) -> Union[str, bytes]:
    # mypy will be RED for the following line
    # indeed, the type-hints do not prevent to x=str + y=bytes, and adding a str to a bytes is forbidden
    return x + y


def concatenate_with_typevar(x: MyType, y: MyType) -> MyType:
    # mypy will be GREEN for the following line
    # indeed, x and y are always of the same type T (T being either str or bytes)
    return x + y


if __name__ == "__main__":
    print(
        """
    The purpose of this POC is to show one situation where defining a type with TypeVar helps.

    In this POC, the 'concatenate_with_union' functions aims to concatenate two texts, that can be either str or bytes.

    As str and bytes are incompatible, the POC shows that using Union[str, bytes] is not enough, because
    it doesn't enforce that both texts are of the same type.

    In contrario, 'concatenate_with_typevar' enforces that both texts are of the same type, either str or bytes.

    What the POC shows :
        - behaviour at runtime is IDENTICAL for both functions (thus the next lines focus on type-checking time)
        - for the nominal situation, mypy is GREEN for both functions
        - for the problematic situation, mypy is wrongfully GREEN for the Union version :-(
        - for the problematic situation, mypy is joyfully RED for the TypeVar version :-)
    """
    )

    # NOMINAL SITUATION = we pass the same types for x and y
    # NOTE : even is mypy is RED for 'concatenate_with_union', at runtime, if x and y are of the same type, all is OK :
    print(concatenate_with_union("coucou", "pouet"))
    print(concatenate_with_union(b"coucou", b"pouet"))

    print(concatenate_with_typevar("coucou", "pouet"))
    print(concatenate_with_typevar(b"coucou", b"pouet"))

    # PROBLEMATIC SITUATION = we pass different types for x and y
    # at runtime, passing DIFFERENT types for x and y leads to an exception for BOTH functions :
    # BUT the error is caught by mypy for 'concatenate_with_typevar' :-)
    try:
        print(concatenate_with_typevar("coucou", b"pouet"))  # mypy detects the error here, yay
    except TypeError:
        print("we got an (expected) TypeError, here 1")
    try:
        print(concatenate_with_union("coucou", b"pouet"))  # mypy doesn't prevent this error :-(
    except TypeError:
        print("we got an (expected) TypeError, here 2")
