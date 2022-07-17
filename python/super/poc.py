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


def invalid_class_hierarchy_because_of_inconsistent_MRO():
    class A:
        pass

    class B:
        pass

    class C(A, B):  # in the MRO, C < A < B
        pass

    class D(A, C):  # in the MRO, D < A < C
        pass


def display_mro_then_explicitly_calling_super():
    class Root:
        def who_am_I(self):
            return "Root"

    class Left(Root):
        def who_am_I(self):
            return "Left"

    class Right(Root):
        def who_am_I(self):
            return "Right"

    class Leaf(Left, Right):
        def who_am_I(self):
            return "Leaf"

    leaf = Leaf()

    def mro_of(klass):
        return " ".join(x.__name__ for x in klass.__mro__)

    print(f"MRO of Leaf = {mro_of(Leaf)}")
    print(f"MRO of Left = {mro_of(Left)}")
    print(f"MRO of Right = {mro_of(Right)}")
    print(f"MRO of Root = {mro_of(Root)}")
    print("")
    print(f"super(Leaf, leaf).who_am_I() = {super(Leaf, leaf).who_am_I()}")
    print(f"super(Left, leaf).who_am_I() = {super(Left, leaf).who_am_I()}")
    print(f"super(Right, leaf).who_am_I() = {super(Right, leaf).who_am_I()}")


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

    print("")
    print("====== Invalid class hierachy because of inconsistent MRO :")
    try:
        invalid_class_hierarchy_because_of_inconsistent_MRO()
    except TypeError as e:
        print("As expected, we got this TypeError :")
        print(f"{type(e).__name__}: {e}")
        # TypeError: Cannot create a consistent method resolution
        # order (MRO) for bases A, C

    print("")
    print("====== Display MRO, then explicitly calling super :")
    display_mro_then_explicitly_calling_super()


if __name__ == "__main__":
    main()
