const {
  promises: { writeFile, readFile, stat, mkdir },
} = require('fs');
const csvWrite = require('csv-stringify');
const csvRead = require('csv-parse');
const b = require('bindings');

const configure = b('configure');
const getCode = b('getCode');
const saveCode = b('saveCode');
const setCode = b('setCode');
const stepCode = b('stepCode');

const getAllCodes = async () => {
  let data = {};
  const inner = async i => {
    const code = await getCode(i);
    data = { ...data, [i]: code };
    // incriments to 12 and then runs one more time
    if (i < 12) inner(i + 1);
  };
  await inner(1);
  return data;
};

const writeCsv = async (obj, ch) => {
  const csv = await new Promise(resolve => csvWrite(Object.entries(obj), (err, data) => resolve(data)));
  // if the local folder doesnt exist, make it
  try {
    await stat('../local');
  } catch (e) {
    await mkdir('../local');
  }
  await writeFile(`../local/ch${ch}default.csv`, csv);
};

const readCsv = async ch => {
  try {
    await stat(`../local/ch${ch}default.csv`);
  } catch (e) {
    throw new Error("That channel doesn't exist or doesn't have a default record");
  }
  const csv = await readFile(`../local/ch${ch}default.csv`, 'utf8');
  const tuples = await new Promise(resolve => csvRead(csv, (err, data) => resolve(data)));
  // convert tuples to an object for easier use
  return tuples.reduce((obj, [key, val]) => ({ ...obj, [key]: +val }), {});
};

module.exports = { configure, getCode, getAllCodes, saveCode, setCode, stepCode, writeCsv, readCsv };
