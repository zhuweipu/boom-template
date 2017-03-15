import random
import itertools

NCONSENSUS=2
NREADS=4

CONSENSUS_LEN=512
READ_LEN=256

def random_sequence(n):
    return ''.join(random.choice(['A', 'T', 'G', 'C']) for i in range(0, n))

def random_bytes(n):
    return [random.randint(0, 255) for i in range(0, n)]

def weighted_hamming_dist(cons, read, qual):
    dist = 0
    for (c, r, q) in zip(cons, read, qual):
        if c != r:
            dist += q
    return dist

def min_weight_pos(cons, read, qual):
    minWeight = 1 << 16
    minPos = 0
    for i in range(0, CONSENSUS_LEN - READ_LEN + 1):
        weight = weighted_hamming_dist(cons[i:i+READ_LEN], read, qual)
        if weight < minWeight:
            minWeight = weight
            minPos = i
    return (minPos, minWeight)

def print_strings(name, data, n, length):
    print("char {}[{}][{}] = {{".format(name, n, length))
    print(",\n".join(["\t\"" + s + "\"" for s in data]))
    print("};")

def print_bytes(name, data, n, length):
    print("unsigned char {}[{}][{}] = {{".format(name, n, length))
    print(",\n".join([
        "\t{" + ", ".join(str(num) for num in row) + "}" for row in data]))
    print("};")

def print_numbers(nbits, name, data, n):
    block_bytes = 64
    word_bytes = nbits / 8
    block_words = block_bytes / word_bytes
    nblocks = (n - 1) / block_words + 1
    nwords = nblocks * block_words
    print("uint{}_t {}[{}] = {{{}}};".format(
            nbits, name, nwords, ", ".join(str(num) for num in data)))

def main():
    consensus = [random_sequence(CONSENSUS_LEN) for i in range(0, NCONSENSUS)]
    reads = [random_sequence(READ_LEN) for i in range(0, NREADS)]
    quality = [random_bytes(READ_LEN) for i in range(0, NREADS)]
    pos_weights = []
    target = random.randint(0, 1 << 16)

    for cons in consensus:
        cons_pos_weights = []
        for (read, qual) in zip(reads, quality):
            cons_pos_weights.append(min_weight_pos(cons, read, qual))
        pos_weights.append(cons_pos_weights)

    ref_cons = pos_weights[0]
    min_cons = []
    min_score = (1 << 32) - 1

    for cons in pos_weights[1:]:
        score = 0
        for ((_, ref_weight), (_, weight)) in zip(ref_cons, cons):
            score += abs(ref_weight - weight)
        if score < min_score:
            min_cons = cons
            min_score = score

    positions = []
    swaps = []
    for ((pos, weight), (_, ref_weight)) in zip(min_cons, ref_cons):
        positions.append(target + pos)
        swaps.append(1 if weight < ref_weight else 0)

    print_strings("refConsensus", consensus, NCONSENSUS, CONSENSUS_LEN)
    print_strings("refReads", reads, NREADS, READ_LEN)
    print_bytes("refQuality", quality, NREADS, READ_LEN)
    print_numbers(32, "refPositions", positions, NREADS)
    print_numbers(8, "refSwaps", swaps, NREADS)

    print("unsigned int refTarget = {};".format(target))
    print("#define NCONSENSUS {}".format(NCONSENSUS))
    print("#define NREADS {}".format(NREADS))

if __name__ == "__main__":
    main()
