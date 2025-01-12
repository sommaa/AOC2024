def make_wire(char, num):
    return f"{char}{str(num).rjust(2, '0')}"

class CircuitVerifier:
    def __init__(self, formulas):
        self.formulas = formulas
        # Pre-compute wire patterns for faster matching
        self.wire_patterns = {i: (
            sorted([make_wire("x", i), make_wire("y", i)]),
            sorted(["x00", "y00"])
        ) for i in range(100)}  # Reasonable upper limit

    def verify_z(self, wire, num):
        if wire not in self.formulas:
            return False
        op, x, y = self.formulas[wire]
        if op != "XOR":
            return False
        if num == 0:
            return sorted([x, y]) == self.wire_patterns[0][1]
        return (self.verify_intermediate_xor(x, num) and self.verify_carry_bit(y, num) or
                self.verify_intermediate_xor(y, num) and self.verify_carry_bit(x, num))

    def verify_intermediate_xor(self, wire, num):
        if wire not in self.formulas:
            return False
        op, x, y = self.formulas[wire]
        if op != "XOR":
            return False
        return sorted([x, y]) == self.wire_patterns[num][0]

    def verify_carry_bit(self, wire, num):
        if wire not in self.formulas:
            return False
        op, x, y = self.formulas[wire]
        if num == 1:
            return op == "AND" and sorted([x, y]) == self.wire_patterns[0][1]
        if op != "OR":
            return False
        num_prev = num - 1
        return (self.verify_direct_carry(x, num_prev) and self.verify_recarry(y, num_prev) or
                self.verify_direct_carry(y, num_prev) and self.verify_recarry(x, num_prev))

    def verify_direct_carry(self, wire, num):
        if wire not in self.formulas:
            return False
        op, x, y = self.formulas[wire]
        return op == "AND" and sorted([x, y]) == self.wire_patterns[num][0]

    def verify_recarry(self, wire, num):
        if wire not in self.formulas:
            return False
        op, x, y = self.formulas[wire]
        if op != "AND":
            return False
        return (self.verify_intermediate_xor(x, num) and self.verify_carry_bit(y, num) or
                self.verify_intermediate_xor(y, num) and self.verify_carry_bit(x, num))

    def verify(self, num):
        return self.verify_z(make_wire("z", num), num)

    def progress(self):
        i = 0
        while self.verify(i):
            i += 1
        return i

def optimize_circuit(formulas):
    verifier = CircuitVerifier(formulas)
    swaps = []
    
    for _ in range(4):
        baseline = verifier.progress()
        improved = False
        
        # Use list of formulas keys to avoid modification during iteration
        keys = list(formulas.keys())
        for i, x in enumerate(keys):
            # Only check combinations we haven't tried yet
            for y in keys[i+1:]:
                formulas[x], formulas[y] = formulas[y], formulas[x]
                if verifier.progress() > baseline:
                    swaps.extend([x, y])
                    improved = True
                    break
                formulas[x], formulas[y] = formulas[y], formulas[x]
            if improved:
                break
        if not improved:
            break
    
    return ",".join(sorted(swaps))

# Main execution
file = open(0)
for line in file:
    if line.isspace(): break
formulas = {}
for line in file:
    x, op, y, z = line.replace(" -> ", " ").split()
    formulas[z] = (op, x, y)

print(optimize_circuit(formulas))
