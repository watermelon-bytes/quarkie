import random
import string

def random_safe_string(length: int) -> str:
    safe = string.ascii_letters + string.digits + "-_. ,:;()[]{}+*/=?!@#$%^&~"
    return "".join(random.choice(safe) for _ in range(length))

patterns = []
    
for _ in range(1400):
    length = random.randint(130, 250)
    patterns.append((f"long_rand_{length}", random_safe_string(length)))

for _ in range(21000):
    length = random.randint(0, 980)
    patterns.append((f"very_long_{length}", random_safe_string(length)))

print("#ifndef STRINGS_TESTING_DATASET_H\n#define STRINGS_TESTING_DATASET_H\n// Automatically generated header by script")
print("static const std::vector<std::tuple<const char*, uint16_t, const char*>> large_collision_test_set = {")

for name, s in patterns:
    escaped = s.replace('\\', '\\\\').replace('"', '\\"')
    print(f'    {{"{escaped}", {len(s)}, "{name}"}},')

print("};\n#endif")
