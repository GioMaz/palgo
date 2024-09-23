const palgoAlgo = async (wasmModule, N, M, maxmg, minw) => {
  const maxmgVec = new wasmModule.IntVec();
  maxmg.forEach(x => {
    maxmgVec.push_back(x);
  });

  const minwVec = new wasmModule.IntVec();
  minw.forEach(x => {
    minwVec.push_back(x);
  });

  const recordVec = wasmModule.palgo(N, M, maxmgVec, minwVec);
  const records = [];
  for (let i = 0; i < recordVec.size(); i++) {
    records.push(recordVec.get(i));
  }

  maxmgVec.delete();
  minwVec.delete();

  return records;
}

export class NamedRecord {
  constructor(name, sets) {
    this.name = name;
    this.sets = sets;
  }
}

export const wrapper = async (wasmModule, days, muscles) => {
  const maxmg = [];
  const minw  = [];
  muscles.forEach(muscle => {
    maxmg.push(muscle.maxmg);
    minw.push(muscle.minw);
  });
  const records = await palgoAlgo(wasmModule, days, muscles.length, maxmg, minw);
  if (records.length == 0) {
    throw new Error("Input constraints are not satisfiable.");
  }

  const namedRecords = Array(days).fill().map(_ => []);
  records.forEach(record => {
    namedRecords[record.g].push(
      new NamedRecord(muscles[record.m].name, record.s)
    );
  });
  return namedRecords;
};

export class Muscle {
  constructor(name, maxmg, minw) {
    this.name   = name;
    this.maxmg  = maxmg;
    this.minw   = minw;
  }
};

const createProgram = async () => {
  const days = 3;
  const muscles = [
    new Muscle("Petto",     12, 24),
    new Muscle("Schiena",   30, 24),
    new Muscle("Spalle",    8,  24),
    new Muscle("Gambe",     12, 12),
    new Muscle("Bicipiti",  12, 24),
    new Muscle("Tricipiti", 8,  12),
  ];

  try {
    const namedRecords = await palgoWrapper(days, muscles);
    console.log(namedRecords);
  } catch (error) {
    console.log(error);
  }
};

