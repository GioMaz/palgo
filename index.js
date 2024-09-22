const palgoAlgo = async (N, M, maxmg, minw) => {
  const factory = require('./palgo.js');
  const instance = await factory();

  const maxmgVec = new instance.IntVec();
  maxmg.forEach(x => {
    maxmgVec.push_back(x);
  });

  const minwVec = new instance.IntVec();
  minw.forEach(x => {
    minwVec.push_back(x);
  });

  const recordVec = instance.palgo(N, M, maxmgVec, minwVec);
  const records = [];
  for (let i = 0; i < recordVec.size(); i++) {
    records.push(recordVec.get(i));
  }
  return records;
}

const palgoWrapper = async (days, muscles) => {
  const maxmg = [];
  const minw  = [];
  muscles.forEach(muscle => {
    maxmg.push(muscle.maxmg);
    minw.push(muscle.minw);
  });
  const records = await palgoAlgo(days, muscles.length, maxmg, minw);
  const namedRecords = Array(days).fill().map(_ => []);
  records.forEach(record => {
    namedRecords[record.g].push({
      name: muscles[record.m].name,
      sets: record.s,
    });
  });
  return namedRecords;
}

const days = 4;
const muscles = [
  { name: "Petto",      maxmg: 12,  minw: 24, },
  { name: "Schiena",    maxmg: 30,  minw: 24, },
  { name: "Spalle",     maxmg: 8,   minw: 24, },
  { name: "Gambe",      maxmg: 12,  minw: 12, },
  { name: "Bicipiti",   maxmg: 12,  minw: 24, },
  { name: "Tricipiti",  maxmg: 8,   minw: 12, },
];

(async () => {
  const namedRecords = await palgoWrapper(days, muscles);
  console.log(namedRecords);
})();
