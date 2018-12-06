import React, { Fragment, Component } from 'react';
import { get, post } from 'axios';
import styled from 'styled-components';
import CodePlot from './codePlot';

const ChannelText = styled.label`
  display: inline-block;
  font-size: 150%;
  margin-left: 40px;
`;

const ChannelRadio = styled.input`
  display: inline-block;
  margin-right: 25px;
  margin-left: 10px;
  transform: scale(1.25);
`;

const SetDefault = styled.button`
  float: right;
  padding: 14px 16px;
  font-size: 125%;
`;

const ApplyDefault = styled.button`
  float: right;
  padding: 14px 16px;
  font-size: 125%;
`;

export default class extends Component {
  constructor(props) {
    super(props);
    this.state = { codes: [], channel: 1 };

    this.handleCodeChange = this.handleCodeChange.bind(this);
    this.handleTempCodeChange = this.handleTempCodeChange.bind(this);
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

  handleTempCodeChange(level, code) {
    const { codes } = this.state;
    codes[12 - level][1] = code;
    this.setState({ codes });
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
          <Fragment key={num}>
            <ChannelText>Channel {num}</ChannelText>
            <ChannelRadio type="radio" checked={channel === num} onChange={this.handleChannelSwitch} value={num} />
          </Fragment>
        ))}
        <SetDefault type="submit" onClick={this.handleSetDefaults}>
          Set Defaults
        </SetDefault>
        <ApplyDefault type="submit" onClick={this.handleApplyDefaults}>
          Apply Defaults
        </ApplyDefault>
        {codes.map(([level, code]) => (
          <CodePlot
            key={level}
            level={level}
            code={code}
            channel={channel}
            handleCodeChange={this.handleCodeChange}
            handleTempCodeChange={this.handleTempCodeChange}
          />
        ))}
      </Fragment>
    );
  }
}
