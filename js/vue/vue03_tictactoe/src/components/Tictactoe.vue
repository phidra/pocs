<script>
import Cell from './Cell.vue';
import {Board} from "../engine.js";

function get_status_line(board) {
  if (board.is_tie()) {
    return "This is a tie, better luck next time !";
  }
  if (!board.has_winner()) {
    return "There is no winner yet...";
  }
  return `WINNER IS ${board.winner}`;
}

export default {
  data() {
    return {
      board: new Board(),
    }
  },
  computed: {
    status_line: function() {
      return get_status_line(this.board);
    },
  },
  methods: {
    on_notified(id) {
      if (this.board.is_over()) {
        // if game is over, we cannot play anymore
        return;
      }

      this.board.cells[id] = this.board.next_mark;
      this.board.switch_next_mark();
    },

    on_start_new_game() {
      this.board.reset();
    },
  },
  components: {
    Cell,
  }
}
</script>

<template>
  <div class="game">
    <div class="game-board">
      <div>
        <div class="status">{{status_line}}</div>
        <div class="board-row">
          <Cell id=0 :value="board.cells[0]" @filled="on_notified" />
          <Cell id=1 :value="board.cells[1]" @filled="on_notified" />
          <Cell id=2 :value="board.cells[2]" @filled="on_notified" />
        </div>
        <div class="board-row">
          <Cell id=3 :value="board.cells[3]" @filled="on_notified" />
          <Cell id=4 :value="board.cells[4]" @filled="on_notified" />
          <Cell id=5 :value="board.cells[5]" @filled="on_notified" />
        </div>
        <div class="board-row">
          <Cell id=6 :value="board.cells[6]" @filled="on_notified" />
          <Cell id=7 :value="board.cells[7]" @filled="on_notified" />
          <Cell id=8 :value="board.cells[8]" @filled="on_notified" />
        </div>
      </div>
    </div>
  </div>
    <button class="start-new-game" @click="on_start_new_game"> Start new game </button>
</template>

<style scoped>
</style>
