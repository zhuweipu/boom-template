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

def min_score_pos(cons, read, qual):
    minScore = 1 << 16
    minPos = 0
    for i in range(0, CONSENSUS_LEN - READ_LEN + 1):
        score = weighted_hamming_dist(cons[i:i+READ_LEN], read, qual)
        if score < minScore:
            minScore = score
            minPos = i
    return (minPos, minScore)

def print_strings(name, data, n, length):
    print("char {}[{}][{}] = {{".format(name, n, length))
    print(",\n".join(["\t\"" + s + "\"" for s in data]))
    print("};")

def print_bytes(name, data, n, length):
    print("unsigned char {}[{}][{}] = {{".format(name, n, length))
    print(",\n".join([
        "\t{" + ", ".join(str(num) for num in row) + "}" for row in data]))
    print("};")

def print_numbers(name, data, n):
    print("unsigned short {}[{}] = {{{}}};".format(
            name, n, ", ".join(str(num) for num in data)))

def main():
    consensus = [random_sequence(CONSENSUS_LEN) for i in range(0, NCONSENSUS)]
    reads = [random_sequence(READ_LEN) for i in range(0, NREADS)]
    quality = [random_bytes(READ_LEN) for i in range(0, NREADS)]
    positions = []
    scores = []

    for (cons, (read, qual)) in itertools.product(consensus, zip(reads, quality)):
        (pos, score) = min_score_pos(cons, read, qual)
        positions.append(pos)
        scores.append(score)

    print_strings("refConsensus", consensus, NCONSENSUS, CONSENSUS_LEN)
    print_strings("refReads", reads, NREADS, READ_LEN)
    print_bytes("refQuality", quality, NREADS, READ_LEN)
    print_numbers("refPositions", positions, NCONSENSUS * NREADS)
    print_numbers("refScores", scores, NCONSENSUS * NREADS)

    print("#define NCONSENSUS {}".format(NCONSENSUS))
    print("#define NREADS {}".format(NREADS))

if __name__ == "__main__":
    main()
