import sys
import itertools
from decimal import Decimal, getcontext
getcontext().prec = 1_001_000

TARGET_PRECISION = 1_000_000


class YEE:
    e_1m_digits: tuple[int, ...]

    def __init__(self):
        # load precalculated e with 1 million decimal precision
        e_1mil = Decimal()
        with open("e.1mil.txt", "r") as f:
            e_1mil = Decimal(f.read().strip().replace("\n", ""))
        self.e_1m_digits = e_1mil.as_tuple().digits

    def count_matching_digits(self, a: Decimal) -> int:
        a_digits = a.as_tuple().digits
        count = 0
        for digit1, digit2 in zip(a_digits, self.e_1m_digits):
            if digit1 == digit2:
                count += 1
            else:
                return count
        return -1

    def method_b(self):
        print("10^n\tcorrect decimal digits\te estimated")
        for i in range(1, 100):
            n = Decimal(f"10e{i}")
            one_over_n = 1 / n
            one_plus_one_over_n = 1 + one_over_n
            e = one_plus_one_over_n ** n

            # -1 to remove the integer part
            correct_digits = self.count_matching_digits(e)-1
            rounded_e = e.__round__(correct_digits)
            print(f"{i}\t{correct_digits}\t\t\t{rounded_e}")

    def method_c(self):
        print("iteration correct_digits")
        e = Decimal(0)
        one_over_fact = Decimal(1)
        one = Decimal(1)

        for i in itertools.count(1):
            e += one_over_fact
            one_over_fact *= one / Decimal(i)
            matching_digits = self.count_matching_digits(e)
            print(i, matching_digits)
            if matching_digits > TARGET_PRECISION:
                exit()

    def method_c_optimized(self):
        e = Decimal(0)
        one_over_fact = Decimal(1)

        last_matching_digits = 0
        for i in itertools.count(1):
            e += one_over_fact
            one_over_fact *= 1 / Decimal(i)

            if i % 100 != 1:
                continue

            matching_digits = self.count_matching_digits(e)
            print(i, matching_digits, matching_digits - last_matching_digits)
            if matching_digits > TARGET_PRECISION:
                exit()

            last_matching_digits = matching_digits


def main():
    yee = YEE()
    method = sys.argv[1]

    if method == "b":
        yee.method_b()
    if method == "c":
        yee.method_c()
    if method == "c_optimized":
        yee.method_c_optimized()


if __name__ == "__main__":
    main()
