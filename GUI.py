import sys
import ast
import re
from PyQt5.QtWidgets import (
    QApplication, QWidget, QStackedLayout, QVBoxLayout, QHBoxLayout,
    QPushButton, QMessageBox, QGridLayout, QLineEdit, QListWidget, QLabel,
    QTextEdit, QSizePolicy, QDialog
)
from PyQt5.QtCore import QProcess, QTimer
from PyQt5.QtGui import QPalette

class GameResultDialog(QDialog):
    def __init__(self, message, color, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Resultado del Juego")
        self.setFixedSize(300, 150)
        self.setModal(True)

        # Set background color
        self.setStyleSheet(f"""
            QDialog {{
                background-color: {color};
                border: 3px solid #333;
                border-radius: 10px;
            }}
            QLabel {{
                color: white;
                font-size: 18px;
                font-weight: bold;
                background-color: transparent;
            }}
        """)

        layout = QVBoxLayout(self)
        layout.setContentsMargins(20, 20, 20, 20)

        label = QLabel(message)
        label.setWordWrap(True)
        label.setStyleSheet("text-align: center;")
        layout.addWidget(label)

        # Auto-close timer
        self.timer = QTimer()
        self.timer.timeout.connect(self.accept)
        self.timer.setSingleShot(True)
        self.timer.start(2000)  # Close after 2 seconds

class RankingDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Ranking de Jugadores")
        self.setFixedSize(600, 400)
        self.setModal(True)

        # Set styling
        self.setStyleSheet("""
            QDialog {
                background-color: #f5f5f5;
                border: 2px solid #333;
                border-radius: 8px;
            }
            QLabel {
                color: #333;
                font-weight: bold;
            }
            QTextEdit {
                background-color: white;
                border: 1px solid #ccc;
                border-radius: 4px;
                font-family: 'Courier New', monospace;
                font-size: 12px;
                padding: 10px;
                color: #333;
            }
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                padding: 8px 16px;
                border-radius: 4px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
        """)

        layout = QVBoxLayout(self)
        layout.setContentsMargins(15, 15, 15, 15)
        layout.setSpacing(10)

        # Title
        title_label = QLabel("🏆 Ranking de Jugadores")
        title_label.setStyleSheet("font-size: 18px; color: #2E7D32; margin-bottom: 10px;")
        layout.addWidget(title_label)

        # Text area for ranking display
        self.ranking_text = QTextEdit()
        self.ranking_text.setReadOnly(True)
        self.ranking_text.setText("Cargando ranking...")
        layout.addWidget(self.ranking_text)

        # Close button
        self.close_btn = QPushButton("Cerrar")
        self.close_btn.clicked.connect(self.accept)
        layout.addWidget(self.close_btn)

    def update_ranking(self, ranking_data):
        """Update the ranking display with formatted data"""
        self.ranking_text.setText(ranking_data)

class MenuWidget(QWidget):
    def __init__(self, switch_to_game, switch_to_ranking, quit_app):
        super().__init__()
        layout = QVBoxLayout(self)
        layout.setSpacing(20)
        layout.addStretch()
        self.play_btn = QPushButton("Jugar (A)")
        self.rank_btn = QPushButton("Ver Ranking (B)")
        self.exit_btn = QPushButton("Salir (C)")

        # Set keyboard shortcuts
        self.play_btn.setShortcut('A')
        self.rank_btn.setShortcut('B')
        self.exit_btn.setShortcut('C')

        for btn, callback in [
            (self.play_btn, switch_to_game),
            (self.rank_btn, switch_to_ranking),
            (self.exit_btn, quit_app)
        ]:
            btn.setFixedHeight(50)
            layout.addWidget(btn)
            btn.clicked.connect(callback)
        layout.addStretch()

class PlayersWidget(QWidget):
    def __init__(self, proc, switch_to_board):
        super().__init__()
        self.proc = proc
        self.switch_to_board = switch_to_board
        layout = QVBoxLayout(self)
        self.instruction = QLabel("Ingrese nombres de jugadores (ingrese 'c' cuando termine):")
        layout.addWidget(self.instruction)
        self.input = QLineEdit(self)
        self.input.setPlaceholderText("Nombre de jugador...")
        layout.addWidget(self.input)
        self.add_btn = QPushButton("Agregar jugador")
        layout.addWidget(self.add_btn)
        self.list_widget = QListWidget(self)
        layout.addWidget(self.list_widget)
        self.start_btn = QPushButton("Comenzar juego")
        layout.addWidget(self.start_btn)

        self.add_btn.clicked.connect(self.add_player)
        self.start_btn.clicked.connect(self.finish_players)
        self.input.returnPressed.connect(self.add_player)

    def reset(self):
        """Reset the widget to initial state"""
        self.list_widget.clear()
        self.input.clear()

    def showEvent(self, event):
        """Called when widget is shown - automatically reset"""
        super().showEvent(event)
        self.reset()
        self.input.setFocus()

    def add_player(self):
        name = self.input.text().strip()
        if not name and self.list_widget.count() != 0:
            self.finish_players()

        if not name:
            return
        self.proc.write((name + "\n").encode())
        self.list_widget.addItem(name)
        self.input.clear()

    def finish_players(self):
        self.proc.write(b"c\n")
        self.proc.write(b"Y\n")  # Confirmar inicio del juego
        self.reset()
        self.switch_to_board()
class BoardWidget(QWidget):
    def __init__(self, proc, switch_to_menu):
        super().__init__()
        self.proc = proc
        self.switch_to_menu = switch_to_menu
        self.cells = []

        # Main layout for the entire widget
        main_layout = QVBoxLayout(self)

        # Grid layout for the game board
        board_layout = QGridLayout()
        board_layout.setSpacing(5)
        for i in range(3):
            row = []
            for j in range(3):
                btn = QPushButton(' ')
                btn.setFixedSize(100, 100)
                btn.setStyleSheet("font-size: 48px;")
                btn.clicked.connect(lambda _, x=i, y=j: self.cell_clicked(x, y))
                board_layout.addWidget(btn, i, j)
                row.append(btn)
            self.cells.append(row)

        # Create a widget to hold the board
        board_widget = QWidget()
        board_widget.setLayout(board_layout)
        main_layout.addWidget(board_widget)

        # Label for remaining games info
        self.remaining_games_label = QLabel("Esperando información del jugador...")
        self.remaining_games_label.setStyleSheet("""
            font-size: 16px; 
            font-weight: bold; 
            color: #333; 
            background-color: #f0f0f0; 
            padding: 8px; 
            border: 1px solid #ccc; 
            border-radius: 4px;
        """)
        self.remaining_games_label.setFixedHeight(40)
        main_layout.addWidget(self.remaining_games_label)

    def cell_clicked(self, i, j):
        if self.proc.state() != QProcess.Running:
            return
        cmd = f"{i+1} {j+1}\n"
        self.proc.write(cmd.encode())

    def process_output(self, data):
        """Process output data to update the board and detect game results"""
        print(f"BoardWidget processing data: {repr(data)}")  # Debug output

        # Check for API message to return to menu
        if "Enviando datos a la API..." in data:
            print("API message detected, switching to menu")
            # Add a small delay to ensure message is processed
            QTimer.singleShot(100, self.switch_to_menu)
            return

        # First, always process board updates
        lines = data.splitlines()
        board_updated = False

        for raw_line in lines:
            line = raw_line.strip()

            # Check for remaining games info - be more flexible with the search
            if "Partidas restantes" in line:
                print(f"Found potential remaining games line: '{line}'")  # Debug

                # Try multiple parsing approaches
                try:
                    # Method 1: Look for the pattern "Partidas restantes de 'UserName': Number"
                    if ":" in line:
                        # Split the line by colon first
                        parts = line.split(":")
                        if len(parts) >= 2:
                            left_part = parts[0].strip()  # "Partidas restantes de 'UserName'"
                            remaining_games = parts[1].strip()  # "2"

                            # Extract username from left part
                            if "Partidas restantes de" in left_part:
                                username_start = left_part.find("Partidas restantes de") + len("Partidas restantes de")
                                username_part = left_part[username_start:].strip()

                                # Remove quotes if present
                                if username_part.startswith("'") and username_part.endswith("'"):
                                    username_part = username_part[1:-1]
                                elif username_part.startswith('"') and username_part.endswith('"'):
                                    username_part = username_part[1:-1]

                                # Update the label
                                self.remaining_games_label.setText(f"Jugador: {username_part} | Partidas restantes: {remaining_games}")
                                self.remaining_games_label.update()  # Force update
                                self.remaining_games_label.repaint()  # Force repaint
                                print(f"SUCCESS: Updated remaining games: '{username_part}' has '{remaining_games}' games left")
                                continue

                    # Method 2: Use regex as fallback
                    pattern = r"Partidas restantes de ['\"]?([^'\":\n]+)['\"]?\s*:\s*(\d+)"
                    match = re.search(pattern, line)
                    if match:
                        username_part = match.group(1).strip()
                        remaining_games = match.group(2).strip()
                        self.remaining_games_label.setText(f"Jugador: {username_part} | Partidas restantes: {remaining_games}")
                        self.remaining_games_label.update()  # Force update
                        self.remaining_games_label.repaint()  # Force repaint
                        print(f"SUCCESS (regex): Updated remaining games: '{username_part}' has '{remaining_games}' games left")
                        continue

                    # Method 3: Just show the raw line for debugging
                    self.remaining_games_label.setText(f"Debug: {line}")
                    print(f"Could not parse, showing raw: '{line}'")

                except Exception as e:
                    print(f"Error parsing remaining games info: {e}")
                    # Show raw line for debugging
                    self.remaining_games_label.setText(f"Parse Error: {line}")
                    import traceback
                    traceback.print_exc()

            if line.startswith('[') and line.endswith(']'):
                try:
                    cells = ast.literal_eval(line)
                    print(f"Parsed cells: {cells}")  # Debug output

                    if isinstance(cells, list) and len(cells) == 9:
                        print("Updating board...")  # Debug output
                        # Update the board - mapping from 1D array to 2D grid
                        for idx, char in enumerate(cells):
                            row = idx // 3  # Integer division to get row (0, 1, or 2)
                            col = idx % 3   # Modulo to get column (0, 1, or 2)

                            # Clean up the character - handle spaces and empty strings
                            if char is None or char == '' or char.isspace():
                                display_char = ' '
                            else:
                                display_char = str(char).strip()
                                if not display_char:
                                    display_char = ' '

                            self.cells[row][col].setText(display_char)
                            print(f"Set cell [{row}][{col}] (index {idx}) to '{display_char}'")  # Debug output

                        print("Board update complete.")
                        board_updated = True
                        # Force GUI update
                        self.update()
                        self.repaint()  # Force immediate repaint

                    else:
                        print(f"Invalid cell data: length={len(cells) if isinstance(cells, list) else 'not a list'}")
                except Exception as e:
                    print(f"Error parsing line '{line}': {e}")
                    import traceback
                    traceback.print_exc()

        # Then check for game result messages
        for line in lines:
            line_stripped = line.strip()
            if "El jugador ha ganado!" in line_stripped:
                # Ensure the board is updated before showing popup
                if board_updated:
                    QApplication.processEvents()  # Process any pending GUI updates
                self.show_game_result("¡El jugador ha ganado!", "#4CAF50")  # Green
                return
            elif "La maquina ha ganado!" in line_stripped:
                # Ensure the board is updated before showing popup
                if board_updated:
                    QApplication.processEvents()  # Process any pending GUI updates
                self.show_game_result("¡La máquina ha ganado!", "#F44336")  # Red
                return
            elif "Empate!" in line_stripped:
                # Ensure the board is updated before showing popup
                if board_updated:
                    QApplication.processEvents()  # Process any pending GUI updates
                self.show_game_result("¡Empate!", "#FF9800")  # Orange/Yellow
                return

    def show_game_result(self, message, color):
        """Show game result popup with colored background"""
        print(f"Showing game result: {message} with color {color}")

        # Force one more GUI update before showing the dialog
        self.update()
        self.repaint()
        QApplication.processEvents()

        dialog = GameResultDialog(message, color, self)
        dialog.exec_()

        # After dialog closes, force another update to ensure board is visible
        print("Game result dialog closed")
        self.update()
        self.repaint()
        QApplication.processEvents()

    def handle_output(self):
        # This method is no longer used - keeping for compatibility
        pass

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Tateti GUI Mejorada con Logs")
        self.resize(700, 480)

        main_layout = QHBoxLayout(self)
        left_layout = QVBoxLayout()

        # Stack de vistas
        self.stack = QStackedLayout()
        left_layout.addLayout(self.stack)

        # Proceso C
        self.proc = QProcess(self)
        self.proc.setProcessChannelMode(QProcess.MergedChannels)
        self.proc.readyReadStandardOutput.connect(self.append_log)

        try:
            self.proc.start("./tateti")
        except Exception:
            self.proc.start("./tateti")

        # Widgets
        self.menu = MenuWidget(
            switch_to_game=self.goto_players,
            switch_to_ranking=self.show_ranking,
            quit_app=QApplication.instance().quit
        )
        self.players = PlayersWidget(self.proc, switch_to_board=self.show_board)
        # Pass the switch_to_menu callback to BoardWidget
        self.board = BoardWidget(self.proc, switch_to_menu=self.switch_to_menu)

        self.stack.addWidget(self.menu)
        self.stack.addWidget(self.players)
        self.stack.addWidget(self.board)

        left_layout.addStretch()
        main_layout.addLayout(left_layout)

        # Logs
        self.log_view = QTextEdit(self)
        self.log_view.setReadOnly(True)
        self.log_view.setSizePolicy(QSizePolicy.Preferred, QSizePolicy.Expanding)
        main_layout.addWidget(self.log_view)

        self.setLayout(main_layout)

        # Initialize ranking capture variables
        self.capturing_ranking = False
        self.ranking_data = []

    def switch_to_menu(self):
        """Switch back to the main menu"""
        print("Switching to main menu")
        self.stack.setCurrentWidget(self.menu)

    def append_log(self):
        """Modified append_log method to handle ranking data capture"""
        data = self.proc.readAllStandardOutput().data().decode()
        self.log_view.append(data)

        # Handle ranking data capture
        if hasattr(self, 'capturing_ranking') and self.capturing_ranking:
            self.process_ranking_data(data)

        # Also handle board updates here
        if hasattr(self, 'board') and self.stack.currentWidget() == self.board:
            self.board.process_output(data)

    def process_ranking_data(self, data):
        """Process ranking data from C program output"""
        lines = data.splitlines()

        # Look for ranking table data
        for line in lines:
            line = line.strip()

            # Skip empty lines
            if not line:
                continue

            # Look for table separator lines or header
            if '|' in line and ('Pos' in line or 'Nombre' in line or line.count('|') >= 4):
                self.ranking_data.append(line)
                continue

            # Look for data rows (should have | separators)
            if '|' in line and line.count('|') >= 4:
                # This looks like a data row
                self.ranking_data.append(line)
                continue

            # Look for lines that might be ranking entries without | separators
            # Try to match pattern like "1   jugUno          2             17/06/2025 23:05:31"
            pattern = r'^\s*(\d+)\s+(\w+)\s+(-?\d+)\s+([\d/:\s]+)\s*$'
            match = re.match(pattern, line)
            if match:
                pos, name, score, date = match.groups()
                formatted_line = f"| {pos:<3} | {name:<15} | {score:<13} | {date:<20} |"
                self.ranking_data.append(formatted_line)

        # Update dialog if we have collected some data
        if len(self.ranking_data) > 0:
            self.update_ranking_display()

    def update_ranking_display(self):
        """Format and display the collected ranking data"""
        if not hasattr(self, 'ranking_dialog') or not self.ranking_data:
            return

        # Create a nicely formatted ranking display
        formatted_ranking = []

        # Add header if not present
        has_header = any('Pos' in line and 'Nombre' in line for line in self.ranking_data)
        if not has_header:
            formatted_ranking.extend([
                "+" + "-" * 58 + "+",
                "| Pos | Nombre          | Puntaje Total | Ultima Partida       |",
                "+" + "-" * 58 + "+"
            ])

        # Add the data
        for line in self.ranking_data:
            if line.strip():
                formatted_ranking.append(line)

        # Add bottom border if needed
        if formatted_ranking and not formatted_ranking[-1].startswith('+'):
            formatted_ranking.append("+" + "-" * 58 + "+")

        # Join and display
        ranking_text = "\n".join(formatted_ranking)

        # Add some summary info
        data_rows = [line for line in self.ranking_data if '|' in line and not ('Pos' in line or line.startswith('+'))]
        if data_rows:
            summary = f"\n\nTotal de jugadores: {len(data_rows)}"
            ranking_text += summary

        self.ranking_dialog.update_ranking(ranking_text)

    def show_ranking(self):
        """Show ranking dialog and request ranking data from C program"""
        if self.proc.state() != QProcess.Running:
            QMessageBox.warning(self, "Error", "El programa C no está ejecutándose")
            return

        # Create and show dialog
        self.ranking_dialog = RankingDialog(self)

        # Set up temporary data capture for ranking
        self.capturing_ranking = True
        self.ranking_data = []

        # Send 'B' command to C program
        self.proc.write(b"B\n")

        # Show dialog
        self.ranking_dialog.exec_()

        # Clean up
        self.capturing_ranking = False
        self.ranking_data = []

    def goto_players(self):
        if self.proc.state() == QProcess.Running:
            self.proc.write(b"A\n")
        self.stack.setCurrentWidget(self.players)

    def show_board(self):
        self.stack.setCurrentWidget(self.board)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = MainWindow()
    win.show()
    sys.exit(app.exec_())