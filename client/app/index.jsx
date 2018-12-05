import React, { Fragment, Component } from 'react';
import { get, post } from 'axios';
import CodePlot from './codePlot';

export default class extends Component {
  constructor(props) {
    super(props);
    this.state = { codes: [], channel: 1 };

    this.handleCodeChange = this.handleCodeChange.bind(this);
    this.handleChannelSwitch = this.handleChannelSwitch.bind(this);
    this.handleSetDefaults = this.handleSetDefaults.bind(this);
    this.handleApplyDefaults = this.handleApplyDefaults.bind(this);
  }

  async componentDidMount() {
    await get('/api/configure');
    await this.getAllCodes();
  }

  async getAllCodes() {
    try {
      const {
        data: { codes },
      } = await get('/api/getAllCodes');
      this.setState({ codes });
    } catch (e) {
      alert(e); // eslint-disable-line no-alert
    }
  }

  async handleCodeChange(level, newCode) {
    const { codes, channel } = this.state;
    try {
      await post('/api/setCode', { level, code: newCode });
      await post('/api/saveCode', { level, code: newCode });
      const {
        data: { code },
      } = await get('/api/getCode', { params: { level } });
      codes[12 - level][1] = code;
      await post('/api/current', { channel, current: codes });
      this.setState({ codes });
    } catch (e) {
      alert(e); // eslint-disable-line no-alert
    }
  }

  handleChannelSwitch({ target: { value } }) {
    this.setState({ channel: +value });
  }

  async handleSetDefaults() {
    const { channel, codes: defaults } = this.state;
    // eslint-disable-next-line no-alert
    if (!window.confirm(`This will set the defaults for channel ${channel} to the current values, are you sure?`))
      return;
    try {
      await post('/api/defaults', { channel, defaults });
    } catch (e) {
      alert(e); // eslint-disable-line no-alert
    }
  }

  async handleApplyDefaults() {
    const { channel } = this.state;
    // eslint-disable-next-line no-alert
    if (!window.confirm(`This will apply the defaults for channel ${channel} to the box, are you sure?`)) return;
    try {
      const {
        data: { defaults: codes },
      } = await get('/api/defaults', { params: { channel } });
      await Promise.all([
        post('/api/setAllCodes', { codes }),
        post('/api/saveAllCodes', { codes }),
        post('/api/current', { channel, current: codes }),
      ]);
      await this.getAllCodes();
    } catch (e) {
      alert(e); // eslint-disable-line no-alert
    }
  }

  render() {
    const { codes, channel } = this.state;
    return (
      <Fragment>
        {[1, 2, 3, 4, 5].map(num => (
          <div key={num}>
            <input type="radio" checked={channel === num} onChange={this.handleChannelSwitch} value={num} />
            Channel {num}
          </div>
        ))}
        <button type="submit" onClick={this.handleSetDefaults}>
          Set Defaults
        </button>
        <button type="submit" onClick={this.handleApplyDefaults}>
          Apply Defaults
        </button>
        {codes.map(([level, code]) => (
          <CodePlot key={level} level={level} code={code} handleCodeChange={this.handleCodeChange} />
        ))}
      </Fragment>
    );
  }
}
