class InfoPanelView {
    constructor(youPanelEl, opponentPanelEl, bannerEl) {
        // document.getElementById('players').y
        this.youPanel = youPanelEl;
        this.opponentPanel = opponentPanelEl;
        this.banner = bannerEl; 

        this.youSymbolEl = this.youPanel.querySelector('.sym');
        this.opponentSymbolEl = this.opponentPanel.querySelector('.sym');
        this.opponentLabelEl = this.opponentPanel.querySelector('#opponentLabel');
    }

    render(viewModel) {
        this.switchTurn();
    }

    setupToggleElement(playerId, currentId, symbol) {
        if (symbol == "O") {
            this.youSymbolEl.innerHTML = "O";
            this.opponentSymbolEl.innerHTML = "X";

            let root = document.querySelector(':root');
            root.style.setProperty('--you', 'var(--color-o)');
            root.style.setProperty('--you-text', 'var(--o-text)');
            root.style.setProperty('--opponent', 'var(--color-x)');
            root.style.setProperty('--opponent-text', 'var(--x-text)');
        } else {
            this.opponentSymbolEl.innerHTML = "O";
        }

        this.opponentLabelEl.innerHTML = "Opponent";
        this.opponentPanel.classList.remove('waiting');

        if (currentId != playerId) {
            this.youPanel.classList.remove('active');
            this.opponentPanel.classList.add('active');
        }
    }

    switchTurn() {
        for (const panel of [this.youPanel, this.opponentPanel])
            if (panel.classList.contains("active")) {
                panel.classList.remove("active");
            } else {
                panel.classList.add("active");
            }
    }

    showResult(isDraw, winner, playerId) {
        this.banner.hidden = false;

        var status;
        if (isDraw) {
            status = "draw";
        } else {
            if (playerId == winner) {
                status = "win";
            } else {
                status = "lose";
            }
        }

        this.banner.classList.add(status);
        this.banner.innerHTML = status.toUpperCase();
    }
}

export default InfoPanelView;