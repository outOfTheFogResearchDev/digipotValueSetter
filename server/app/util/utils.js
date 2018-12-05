const {
  promises: { writeFile, readFile, stat, mkdir },
} = require('fs');
const csvWrite = require('csv-stringify');
const csvRead = require('csv-parse');
const b = require('bindings');

const configure = b('configure'); // configures digipot, invoke once per connection to digipot
const getCode = b('getCode'); // (level (Number)) returns the code for that level
const setCode = b('setCode'); // (level (Number), code (Number)) sets the code to the level in temp memory
const saveCode = b('saveCode'); // (level (Number), code (Number)) saves the code to the level
const stepCode = b('stepCode'); // (level (Number), (+1 or -1)) steps the code in the level up 1 or down 1 in temp memory

const getAllCodes = async () => {
  let data = {};
  const inner = async i => {
    const code = await getCode(i);
    data = { ...data, [i]: code };
    // incriments to 1 -> 12
    if (i < 12) await inner(i + 1);
  };
  await inner(1);
  return data;
};

const setAllCodes = async codes => {
  const inner = async i => {
    const [channel, code] = codes[i];
    await setCode(channel, code);
    // incriments to 0 -> 11
    if (i < 12 - 1) await inner(i + 1);
  };
  await inner(0);
};

const saveAllCodes = async codes => {
  const inner = async i => {
    const [channel, code] = codes[i];
    await saveCode(channel, code);
    // incriments to 0 -> 11
    if (i < 12 - 1) await inner(i + 1);
  };
  await inner(0);
};

const defaultCsvFolderLocation = './server/app/local';
const defaultCsvLocation = channel => `${defaultCsvFolderLocation}/channel${channel}default.csv`;

const writeCsv = async (channel, tuples) => {
  const csv = await new Promise(resolve => csvWrite(tuples.sort((x, y) => x[0] - y[0]), (err, data) => resolve(data)));
  // if the local folder doesnt exist, make it
  try {
    await stat(defaultCsvFolderLocation);
  } catch (e) {
    await mkdir(defaultCsvFolderLocation);
  }
  await writeFile(defaultCsvLocation(channel), csv);
};

const readCsv = async channel => {
  // check to see if that channel has a default file
  try {
    await stat(defaultCsvLocation(channel));
  } catch (e) {
    throw new Error("That channel doesn't exist or doesn't have a default record");
  }
  const csv = await readFile(defaultCsvLocation(channel), 'utf8');
  return new Promise(resolve => csvRead(csv, (err, data) => resolve(data.sort((x, y) => x[0] - y[0]))));
};

module.exports = {
  configure,
  getCode,
  getAllCodes,
  setCode,
  setAllCodes,
  saveCode,
  saveAllCodes,
  stepCode,
  writeCsv,
  readCsv,
};
