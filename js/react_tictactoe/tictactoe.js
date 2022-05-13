'use strict';

function Square(props) {
	return (
	<button
		className="square"
		onClick={ props.clickHandler }
	>
		{props.value}
	</button>
	);
}

class Board extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			board: [
				null, null, null,
				null, null, null,
				null, null, null,
			]
		};
	};
    renderSquare(i) {
        return (
			<Square
				value={this.state.board[i]}
				clickHandler={() => this.handleSquareClick(i)}
				// quelques explications : on crée le composant Square, en lui passant des props
				// la props "clickHandler" est une fonction qui ne prend pas d'argument, et appelle une autre fonction en figeant son argument
				// cette second fonction capture (closure) le state du board, et peut donc le modifier
				// du coup, pour un square donné, le clickHandler est une simple fonction, qui a pour effet de modifier l'état du board
			/>);
    }
	handleSquareClick(i) {
		// only update empty cases :
		let newBoard = this.state.board.slice();
		if (this.state.board[i] === null) {
			newBoard[i] = 'X';
		}
		else {
			newBoard[i] = null;
		}

		this.setState({board: newBoard});
	}

  render() {
    const status = 'Next player: X';

    return (
      <div>
        <div className="status">{status}</div>
        <div className="board-row">
          {this.renderSquare(0)}
          {this.renderSquare(1)}
          {this.renderSquare(2)}
        </div>
        <div className="board-row">
          {this.renderSquare(3)}
          {this.renderSquare(4)}
          {this.renderSquare(5)}
        </div>
        <div className="board-row">
          {this.renderSquare(6)}
          {this.renderSquare(7)}
          {this.renderSquare(8)}
        </div>
      </div>
    );
  }
}

function Game(props)
{
    return (
      <div className="game">
        <div className="game-board">
          <Board />
        </div>
        <div className="game-info">
          <div>{/* status */}</div>
          <ol>{/* TODO */}</ol>
        </div>
      </div>
    );
}

// ========================================

ReactDOM.render(<Game />, document.getElementById('root'));
