const {
  promises: { writeFile, readFile, stat, mkdir },
} = require('fs');
const csvWrite = require('csv-stringify');
const csvRead = require('csv-parse');
const b = require('bindings');

const configure = b('configure'); // configures digipot, invoke once per connection to digipot
const getCode = b('getCode'); // (level (Number)) returns the code for that level
const saveCode = b('saveCode'); // (level (Number), code (Number)) saves the code to the level
const setCode = b('setCode'); // (level (Number), code (Number)) sets the code to the level in temp memory
const stepCode = b('stepCode'); // (level (Number), (+1 or -1)) steps the code in the level up 1 or down 1 in temp memory

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

const defaultCsvFolderLocation = './server/app/local';
const defaultCsvLocation = channel => `${defaultCsvFolderLocation}/channel${channel}default.csv`;

const writeCsv = async (channel, obj) => {
  const csv = await new Promise(resolve => csvWrite(Object.entries(obj), (err, data) => resolve(data)));
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
  const tuples = await new Promise(resolve => csvRead(csv, (err, data) => resolve(data)));
  // convert tuple array to object for easier use
  return tuples.reduce((obj, [key, val]) => ({ ...obj, [key]: +val }), {});
};

module.exports = { configure, getCode, getAllCodes, saveCode, setCode, stepCode, writeCsv, readCsv };
