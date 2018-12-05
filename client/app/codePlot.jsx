import React, { Component } from 'react';
import styled from 'styled-components';

const LevelForm = styled.form`
  padding: 5px 5px;
  margin: 10px 0;
  box-sizing: border-box;
`;

const LevelText = styled.label`
  display: inline-block;
  font-size: 125%;
  margin-left: 1%;
`;

const LevelText = styled.input`
  display: inline-block;
  width: 50%;
  in-right: 3%;
`;

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
      <LevelForm>
        <LevelText>Level {level}:</LevelText>
        <LevelText type="number" min="90" max="200" value={code} onChange={this.handleChange} />
      </LevelForm>
    );
  }
}
