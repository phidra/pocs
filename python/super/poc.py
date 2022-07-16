#!/usr/bin/env python3


def nominal_case():
    class Parent:
        def hello(self, name: str):
            print(f"Hello from Parent, {name}")

    class Child(Parent):
        def hello(self, name: str):
            super().hello(name)  # <-- appellera Parent.hello(name)
            print(f"And also hello from Child, {name}")

    child = Child()
    child.hello("Anakin")


def super_actually_calls_sibling():
    class Root:
        def f(self):
            print("from Root")

    class A(Root):
        def f(self):
            print("vvv Below is the interesting part : A.f is about to call super...")
            print("    ... but even if A's parent is Root, super will actually call B (which is the SIBLING of A")
            super().f()
            print("^^^ above was the call to super made by A.f")
            print("from A")

    class B(Root):
        def f(self):
            print("Here is another interesting part : as B.f does NOT use super, Root.f is never called !")
            print("from B")

    class C(A, B):
        def f(self):
            super().f()
            print("from C")

    c = C()
    c.f()


def super_actually_calls_grandparent():
    class GrandParent:
        def f(self):
            print("from GrandParent")

    class Parent(GrandParent):
        pass  # pas de méthode f !

    class Child(Parent):
        def f(self):
            super().f()
            print("from Child")

    child = Child()
    child.f()


def main() -> None:
    print("")
    print("====== Nominal case :")
    nominal_case()

    print("")
    print("====== Super actually calls sibling :")
    super_actually_calls_sibling()

    print("")
    print("====== Super actually calls grand parent :")
    super_actually_calls_grandparent()


if __name__ == "__main__":
    main()
