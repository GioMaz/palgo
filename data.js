const setupDatabase = db => {
  const usersQuery = `CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT
  )`;
  db.exec(usersQuery);
  const nrsQuery = `CREATE TABLE IF NOT EXISTS nrs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    userId INTEGER,
    namedRecords TEXT,
    FOREIGN KEY(userId) REFERENCES users(id)
  )`;
  db.exec(nrsQuery);
};

const insertUser = async db => {
  const query = `INSERT INTO users DEFAULT VALUES`;
  const { lastID } = await db.run(query);
  return lastID;
};

const getUser = async (db, id) => {
  const query = `SELECT id
    FROM users
    WHERE id = :id`;
  const result = await db.get(query, {
    ':id': id
  });
  return result;
};

const insertNamedRecords = async (db, userId, namedRecords) => {
  const query = `INSERT INTO nrs(userId, namedRecords)
    VALUES(:userId, :namedRecords)`;
  return await db.run(query, userId, JSON.stringify(namedRecords));
};

const getNamedRecords = async (db, id) => {
  const query = `SELECT namedRecords
    FROM users
    INNER JOIN nrs
    ON users.id = nrs.userId
    WHERE users.id = :id`;
  const nrs = await db.all(query, {
    ':id': id
  });
  return nrs.map(x => JSON.parse(x.namedRecords));
};

module.exports = {
  setupDatabase,
  insertUser,
  getUser,
  insertNamedRecords,
  getNamedRecords
};
