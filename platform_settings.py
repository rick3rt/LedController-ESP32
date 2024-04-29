import sys
Import("env")

IS_WINDOWS = sys.platform.startswith("win")
IS_LINUX = sys.platform.startswith("linux")
IS_MAC = sys.platform.startswith("darwin")

# should be absolute paths, not relative ones
if IS_WINDOWS:
    monitor_port = "COM*"
elif IS_LINUX:
    monitor_port = "/dev/ttyUSB*"
else:
    sys.stderr.write("Unrecognized OS.\n")
    env.Exit(-1)

print("Replacing monitor port with %s." % monitor_port)
env.Replace(
    MONITOR_PORT=monitor_port
)
