import subprocess
import os
import time

test_files = sorted(os.listdir("inputs"))
compile_command = ["g++", "main.cpp", "-o", "main"]
subprocess.run(compile_command, check=True)

mean_time = 0
total_time = 0
iterations = 1000
for i in range(iterations):
    time_spent = time.time()
    for test_file in test_files:
        execute_command = ["./main"]
        with open(f"inputs/{test_file}", "r") as input_file:
            output = subprocess.run(execute_command, stdin=input_file, stdout=subprocess.PIPE, text=True)

        # Now, output.stdout contains the output of the executed program
        output = output.stdout.rstrip()
        expected_output = open(f"correct_outputs/{test_file}").read().rstrip()

        if(i == 0):
            if output == expected_output:
                print(f"Test {test_file} passed")
            else:
                print(f"Test {test_file} failed")
                break

    time_spent = time.time() - time_spent
    mean_time += time_spent
    total_time += time_spent

mean_time /= iterations
print(f"Mean time spent: {mean_time:.4f} seconds")
print(f"Total time spent: {total_time:.4f} seconds")

# Mean time spent: 0.0076 seconds
# Total time spent: 7.5739 seconds