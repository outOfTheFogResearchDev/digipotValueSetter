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
  const data = [];
  const inner = async i => {
    const code = await getCode(i);
    data.push([i, code]);
    if (i > 1) await inner(i - 1);
  };
  await inner(12);
  return data;
};

const setAllCodes = async codes => {
  const inner = async i => {
    const [level, code] = codes[i];
    await setCode(+level, +code);
    if (i < 11) await inner(i + 1);
  };
  await inner(0);
};

const saveAllCodes = async codes => {
  const inner = async i => {
    const [level, code] = codes[i];
    await saveCode(+level, +code);
    if (i < 11) await inner(i + 1);
  };
  await inner(0);
};

const csvFolderLocation = './server/local';
const csvLocation = (channel, type) => `${csvFolderLocation}/channel${channel}${type}.csv`;

const writeCsv = async (channel, type, tuples) => {
  tuples.sort((x, y) => +y[0] - +x[0]);
  const csv = await new Promise(resolve => csvWrite(tuples, (err, data) => resolve(data)));
  // if the local folder doesnt exist, make it
  try {
    await stat(csvFolderLocation);
  } catch (e) {
    await mkdir(csvFolderLocation);
  }
  await writeFile(csvLocation(channel, type), csv);
};

const readCsv = async (channel, type) => {
  // check to see if that channel has a default file
  try {
    await stat(csvLocation(channel, type));
  } catch (e) {
    throw new Error("That channel doesn't exist or doesn't have a default record");
  }
  const csv = await readFile(csvLocation(channel, type), 'utf8');
  return new Promise(resolve => csvRead(csv, (err, data) => resolve(data)));
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
