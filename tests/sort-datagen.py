import random

NUM_ELEMENTS = 24

def print_numbers(nbits, name, data):
    n = len(data)
    block_bytes = 64
    word_bytes = nbits / 8
    block_words = block_bytes / word_bytes
    nblocks = (n - 1) / block_words + 1
    nwords = nblocks * block_words
    print("uint{}_t {}[{}] = {{{}}};".format(
            nbits, name, nwords, ", ".join(str(num) for num in data)))

def main():
    #keys1 = [random.randint(0, 0xff) for i in range(0, NUM_ELEMENTS)]
    keys1 = [random.randint(0, 0xff) for i in range(0, NUM_ELEMENTS)]
    keys2 = [random.randint(0, 0xffff) for i in range(0, NUM_ELEMENTS)]
    payload = [random.randint(0, 0xffffffff) for i in range(0, NUM_ELEMENTS)]
    merged = zip(keys1, keys2, payload)
    merged.sort(key=lambda tup: tup[0:2])

    print("#define NUM_ELEMENTS {}".format(NUM_ELEMENTS))
    print_numbers(8, "input_keys1", keys1)
    print_numbers(16, "input_keys2", keys2)
    print_numbers(32, "input_payload", payload)

    [keys1, keys2, payload] = zip(*merged)
    print_numbers(8, "expected_keys1", keys1)
    print_numbers(16, "expected_keys2", keys2)
    print_numbers(32, "expected_payload", payload)

if __name__ == "__main__":
    main()
