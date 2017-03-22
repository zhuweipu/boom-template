import random
import math

def random_quality(n):
    return [random.randint(0, 0xff) for i in range(0, n)]

def print_quality(name, quals):
    print("unsigned char {}[{}][{}] = {{".format(
        name, len(quals), len(quals[0])))
    print(',\n'.join(
            ['\t{' + ', '.join(str(q) for q in qual) + '}' for qual in quals]))
    print("};")

def print_result(name, result):
    print("unsigned int {}[{}] = ".format(name, len(result)) +
            "{" + ', '.join(str(r) for r in result) + "};")

def main():
    QUAL_SIZE = 16
    QUAL_LEN = 4
    inputQuality = [random_quality(QUAL_SIZE) for i in range(0, QUAL_LEN)]
    expectedSum = [sum(qual) for qual in inputQuality]
    expectedMin = [min(qual) for qual in inputQuality]
    expectedMax = [max(qual) for qual in inputQuality]

    print("#define QUAL_SIZE {}".format(int(math.log(QUAL_SIZE, 2))))
    print("#define QUAL_LEN {}".format(QUAL_LEN))
    print_quality("inputQuality", inputQuality)
    print_result("expectedSum", expectedSum)
    print_result("expectedMin", expectedMin)
    print_result("expectedMax", expectedMax)

if __name__ == "__main__":
    main()
