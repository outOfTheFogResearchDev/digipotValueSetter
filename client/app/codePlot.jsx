import React, { Component } from 'react';

export default class extends Component {
  constructor(props) {
    super(props);
    const { level, code } = this.props; // eslint-disable-line react/prop-types
    this.state = { level, code };

    this.handleChange = this.handleChange.bind(this);
  }

  async handleChange({ target: { value: code } }) {
    const { handleCodeChange } = this.props; // eslint-disable-line react/prop-types
    const { level } = this.state;
    if (code >= 90 && code <= 200) await handleCodeChange(level, code);
    this.setState({ code });
  }

  render() {
    const { level, code } = this.state;
    return (
      <form>
        level {level}: <input type="number" min="90" max="200" value={code} onChange={this.handleChange} />
      </form>
    );
  }
}
