import ctypes as ct

from palgo import palgo_named_records, Muscle, NamedRecord

palgo = ct.CDLL("../palgo/libpalgo.so")

if __name__ == "__main__":
    days = 3;
    muscles = [
        Muscle("Petto",     11, 22),
        Muscle("Schiena",   30, 20),
        Muscle("Spalle",    8,  20),
        Muscle("Gambe",     20, 20),
        Muscle("Bicipiti",  10, 23),
        Muscle("Tricipiti", 8,  16),
    ];
    days = palgo_named_records(palgo, days, muscles)
    for day, exercises in enumerate(days):
        for exercise in exercises:
            print(day, str(exercise))
