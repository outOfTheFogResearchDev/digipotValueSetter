import React, { Component } from 'react';

export default class extends Component {
  constructor(props) {
    super(props);

    this.handleChange = this.handleChange.bind(this);
  }

  async handleChange({ target: { value: code } }) {
    const { handleCodeChange, handleTempCodeChange, level } = this.props; // eslint-disable-line react/prop-types
    if (code >= 90 && code <= 200) await handleCodeChange(level, code);
    else handleTempCodeChange(level, code);
  }

  render() {
    const { level, code } = this.props; // eslint-disable-line react/prop-types
    return (
      <form>
        level {level}: <input type="number" min="90" max="200" value={code} onChange={this.handleChange} />
      </form>
    );
  }
}
