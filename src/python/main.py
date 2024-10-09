import ctypes as ct

from palgo import palgo_named_records, Muscle, NamedRecord

palgo = ct.CDLL("../palgo/libpalgo.so")

if __name__ == "__main__":
    n = 4
    muscles = [
        Muscle("Schiena",   30, 20),
        Muscle("Tricipiti", 8,  16),
        Muscle("Spalle",    8,  20),
        Muscle("Petto",     11, 22),
        Muscle("Bicipiti",  10, 23),
        Muscle("Gambe",     20, 2)
    ]
    days = palgo_named_records(palgo, n, muscles)
    for day, exercises in enumerate(days):
        for exercise in exercises:
            print(day, str(exercise))
