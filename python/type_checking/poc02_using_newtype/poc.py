from dataclasses import dataclass
from typing import NewType, Union


print(
    """

This POC shows how to use typing.NewType to define "strong ints".

We use it to distinguish between a rectangle's width and height.

Consequently, mypy finds some bugs in the code :
- but only in the class that uses strong ints = SafeRectangle
- the equivalent bugs in UnsafeRectangle (that DOES NOT use strong ints) are not detected by mypy

WARNING : as for all type-checking, runtime is not affected... and is therefore bugged, in our case.

"""
)


@dataclass
class UnsafeRectangle:
    """'Unsafe' because mypy doesn't detect inversion between width and height at construction."""

    width: int
    height: int


Width = NewType("Width", int)
Height = NewType("Height", int)


@dataclass
class SafeRectangle:
    """'Safe' because mypy DO detect inversion between width and height at construction."""

    width: Width
    height: Height


def print_is_stable(rect: Union[SafeRectangle, UnsafeRectangle]):
    """A rectangle is stable if its width is bigger than its height."""
    print(f"is {rect} stable ? {rect.width >= rect.height}")


width = 5
height = 3
typed_width = Width(width)
typed_height = Height(height)


# unsafe class :
r_unsafe_ok = UnsafeRectangle(width, height)  # <-- OK : no inversion here
r_unsafe_ko = UnsafeRectangle(height, width)  # <-- UNDETECTED BUG : inversion between width and height !

# safe class :
r_safe_ok = SafeRectangle(typed_width, typed_height)  # <-- OK : no inversion here
r_safe_ko1 = SafeRectangle(typed_height, typed_width)  # <-- DETECTED BUG : inversion between width and height :-)

# as SafeRectangle now expects Width/Height (which are not ints), we cannot use 'ints' anymore :
r_safe_ok_ish = SafeRectangle(width, height)  # mypy will (logically) complain, but still no bug at runtime

print("Here, if we didn't introduce some bugs by inverting width and height, all those calls would return True.")
print("Consequently, all the lines that print 'False' are bugged : one is detected by mypy, one is not")
print_is_stable(r_unsafe_ok)
print_is_stable(r_unsafe_ko)
print_is_stable(r_safe_ok)
print_is_stable(r_safe_ko1)
print_is_stable(r_safe_ok_ish)
