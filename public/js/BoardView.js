class BoardView {
    constructor(boardElement) {
        this.board = boardElement;
        this.cells = boardElement.querySelectorAll(".cell");

        this.cells.forEach(cell => {
            cell.addEventListener("click", () => this.onCellClick(cell));
        });

        this.onMoveAttempt = null;
    }

    render(viewModel) {
        for (var i = 0; i < viewModel.board.length; i++) {
            this.cells[i].innerHTML = viewModel.board[i];
            if (viewModel.board[i] == "O") {
                this.cells[i].style.color = "var(--color-o)";
            } else if (viewModel.board[i] == "X") {
                this.cells[i].style.color = "var(--color-x)";
            }
        }

        this.lock(viewModel.playerId == viewModel.currentPlayerId);
    }

    onCellClick(cell) {
        if (this.board.classList.contains('locked')) return;

        this.onMoveAttempt(Number(cell.dataset.index));
    }

    lock(isYourTurn) {
        if (!isYourTurn) {
            this.board.classList.add("locked");
        } else {
            this.board.classList.remove("locked");
        }
    }
}

export default BoardView;