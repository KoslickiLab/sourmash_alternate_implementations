import subprocess
import re

def run_command_and_record_time_memory(command: str):
    """
    Runs a command and measures CPU time, wall-clock time, and peak memory usage using /usr/bin/time -v.

    Args:
        command (str): The command to run.

    Returns:
        dict: A dictionary containing 'cpu_time' (s), 'wall_clock_time' (s), and 'peak_memory_usage' (GB).
    """
    time_command = f"/usr/bin/time -v {command}"
    
    try:
        # Run the command and capture stderr (where /usr/bin/time -v writes its output)
        result = subprocess.run(time_command, shell=True, stderr=subprocess.PIPE, text=True)
        stderr_output = result.stderr
        
        # Regex patterns to extract relevant fields
        cpu_time_pattern = r"User time \(seconds\):\s+(\d+\.\d+)"
        wall_clock_pattern = r"Elapsed \(wall clock\) time \(h:mm:ss or m:ss\):\s+(\S+)"
        peak_memory_pattern = r"Maximum resident set size \(kbytes\):\s+(\d+)"
        
        # Extract data
        cpu_time = float(re.search(cpu_time_pattern, stderr_output).group(1))
        wall_clock = re.search(wall_clock_pattern, stderr_output).group(1)
        peak_memory_kb = int(re.search(peak_memory_pattern, stderr_output).group(1))
        
        # Convert wall-clock time to seconds
        wall_clock_time = convert_wall_clock_to_seconds(wall_clock)
        
        # Convert memory from KB to GB
        peak_memory_gb = peak_memory_kb / (1024 * 1024)
        
        return {
            'cpu_time': cpu_time,
            'wall_clock_time': wall_clock_time,
            'peak_memory_usage': peak_memory_gb
        }
    except Exception as e:
        print(f"Error running command: {e}")
        return None

def convert_wall_clock_to_seconds(wall_clock: str) -> float:
    """
    Converts wall-clock time in h:mm:ss, m:ss, or s format to seconds.

    Args:
        wall_clock (str): The wall clock time as a string.

    Returns:
        float: Wall-clock time in seconds.
    """
    parts = wall_clock.split(':')
    if len(parts) == 3:  # h:mm:ss
        hours, minutes, seconds = map(float, parts)
        return hours * 3600 + minutes * 60 + seconds
    elif len(parts) == 2:  # m:ss
        minutes, seconds = map(float, parts)
        return minutes * 60 + seconds
    else:  # s
        return float(parts[0])

# Example Usage
if __name__ == "__main__":
    command = "sleep 2"  
    metrics = run_command_and_record_time_memory(command)
    if metrics:
        print(f"CPU Time (s): {metrics['cpu_time']}")
        print(f"Wall Clock Time (s): {metrics['wall_clock_time']}")
        print(f"Peak Memory Usage (GB): {metrics['peak_memory_usage']:.3f}")
