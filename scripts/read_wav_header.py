try:
    print("Reading")
    with open("snare.wav", "rb") as f:
        header = f.read(44)
        num_channels = int.from_bytes(header[22:24], "little")
        bits_per_sample = int.from_bytes(header[34:36], "little")
        sample_rate = int.from_bytes(header[24:28], "little")

        print(f"Channels: {num_channels}")
        print(f"Bit depth: {bits_per_sample}")
        print(f"Sample rate: {sample_rate}")
except Exception as e:
    print("Error:", e)