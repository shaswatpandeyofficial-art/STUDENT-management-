"""Exercise the real executable through stdin/stdout using only the stdlib."""
import subprocess
import sys

binary = sys.argv[1]

def run(text):
    result = subprocess.run([binary], input=text, text=True, capture_output=True, timeout=5)
    assert result.returncode == 0, result.stderr
    return result.stdout

output = run("2\n1\n1\nAda Lovelace\n20\n95.5\n1\n1\n3\n1\n4\n1\nAda Updated\n21\n99\n2\n5\n1\n3\n1\n0\n")
for expected in ("No students registered.", "already exists", "Name: Ada Lovelace",
                 "Name: Ada Updated | Age: 21 | Grade: 99.00", "Student not found.", "Goodbye."):
    assert expected in output, (expected, output)
output = run("junk\n1.5\n9\n1\n-1\n1\n   \n" + "x" * 200 + "\nGrace Hopper\nabc\n151\n30\nnan\ninf\n101\n87.25\n2\n0\n")
assert "Input too long" in output
assert "Name must not be blank" in output
assert "Name: Grace Hopper | Age: 30 | Grade: 87.25" in output
assert "Goodbye" in run("")
assert "Success." not in run("1\n1\nIncomplete\n")
assert "Name: " + "x" * 79 in run("1\n1\n" + "x" * 79 + "\n20\n0\n2\n0\n")
print("All CLI integration tests passed.")
