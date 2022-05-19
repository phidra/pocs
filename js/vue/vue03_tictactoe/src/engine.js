const HORIZONTAL1  = [0, 1, 2];
const HORIZONTAL2  = [3, 4, 5];
const HORIZONTAL3  = [6, 7, 8];
const VERTICAL1  = [0, 3, 6];
const VERTICAL2  = [1, 4, 7];
const VERTICAL3  = [2, 5, 8];
const DESCENDING_DIAGONAL  = [0, 4, 8];
const ASCENDING_DIAGNOAL  = [2, 4, 6];
const LINES = [
  HORIZONTAL1,
  HORIZONTAL2,
  HORIZONTAL3,
  VERTICAL1,
  VERTICAL2,
  VERTICAL3,
  DESCENDING_DIAGONAL,
  ASCENDING_DIAGNOAL,
]

class Board {
  constructor() {
    this.cells = ["", "", "", "", "", "", "", "", ""];
    this.next_mark = "O";
  };

  reset() {
    this.cells.fill("");
    this.next_mark = "O";
  };

  switch_next_mark() {
    this.next_mark = this.next_mark === "O" ? "X" : "O";
  }

  is_full() {
    if (this.cells.includes("")) {
      return false;
    }
    return true;
  }

  is_tie() {
    // if board is not full, game is not tie :
    if (!this.is_full()) {
      return false;
    }

    // if game is over, game is a tie if there is no winner :
    return !this.has_winner();
  }

  get winner() {
    for (const cell_indexes of LINES) {
      const val1 =this.cells[cell_indexes[0]];
      const val2 =this.cells[cell_indexes[1]];
      const val3 =this.cells[cell_indexes[2]];
      if (val1 === "") {
        continue;
      }
      if (val1 === val2 && val2 === val3) {
        return val1;
      }
    }
    return null;
  }

  is_over() {
    return this.has_winner() || this.is_tie();
  }

  has_winner() {
    return this.winner !== null;
  }


};

export {
  Board,
}
