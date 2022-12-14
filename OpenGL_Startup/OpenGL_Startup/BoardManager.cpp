#include "BoardManager.h"

BoardManager::BoardManager(Renderer* rend)
{
    std::cout << "[Debug] BoardManager initialize.\n";
    blackModel = std::make_shared<Model>("./resource/model/black_chess/black_chess.obj");
    whiteModel = std::make_shared<Model>("./resource/model/white_chess/white_chess.obj");

    this->_board = vector<vector<Chess*>>(8, vector<Chess*>(8, nullptr));

    _gameStatus = GameStatus::Preparing;
    _rightMouseState = MouseState::Idle;
    std::cout << "[Debug] BoardManager initialize success.\n";

    gameStart = false;
    WhiteBot = false;
    BlackBot = false;
    whiteOutOfMove = false;
    blackOutOfMove = false;
    aiSpeed = 1.5;

    Chess c(Chess::Color::White, blackModel, whiteModel);
}

BoardManager::~BoardManager()
{
    clearChess();
}

glm::vec3 BoardManager::boardIdx2WorldPos(const int& x, const int& y)
{
    return glm::vec3(-0.88 + x * 0.25, 0.12, -0.88 + y * 0.25);
}

void BoardManager::setChessStatus(const int& x, const int& y, unsigned int status, bool flag)
{
    if (0 <= x && x < BOARD_SIZE_X && 0 <= y && y < BOARD_SIZE_Y && this->_board[y][x]) {
        this->_board[y][x]->setStatus(status, flag);
    }
    else {
        std::cout << "[Debug] Set Chess Status Failed \n";
    }
}

void BoardManager::addChess(const int& x, const int& y, const Chess::Color& color)
{
    if (0 <= x && x < BOARD_SIZE_X && 0 <= y && y < BOARD_SIZE_Y && !this->_board[y][x]) {
        //std::cout << "[Debug] Add Chess in: (" << x << " , " << y << "), color: " << int(color) << "\n";
        this->_board[y][x] = newChess(color);
        //std::cout << "[Debug] Set Chess Transform: (" << x << " , " << y << "), color: " << int(color) << "\n";
        this->_board[y][x]->m_transform.position = boardIdx2WorldPos(y, x);
        this->_board[y][x]->setStatus(Chess::Status::fliping, true);
    }
    else {
        std::cout << "[Debug] Add Chess Failed \n";
    }
}

void BoardManager::findAvailablePos(const Chess::Color& color)
{

    bool isEnd = false;
    while (!isEnd) {
        isEnd = true;
        for (int i = 0; i < BOARD_SIZE_Y; i++) {
            for (int j = 0; j < BOARD_SIZE_X; j++) {
                Chess* chess = this->_board[j][i];
                if (!chess || chess->getColor() != color || chess->getStatus(Chess::Status::Hint)) continue;

                for (int dirIdx = 0; dirIdx <= 7; dirIdx++) {
                    glm::ivec2 dir = { DirTable[dirIdx][0], DirTable[dirIdx][1] };
                    glm::ivec2 currentPos = glm::ivec2(i, j);
                    currentPos += dir;
                    Chess* tmpChess = getChess(currentPos.x, currentPos.y);

                    if (!tmpChess) {
                        continue;
                    }

                    // check available: different color in the middle.
                    while (tmpChess && tmpChess->getColor() != color) {

                        currentPos += dir;
                        tmpChess = getChess(currentPos.x, currentPos.y);

                    }
                    if (!tmpChess && currentPos - dir != glm::ivec2(i, j)) {

                        if (!(0 <= currentPos.x && currentPos.x < BOARD_SIZE_X && 0 <= currentPos.y && currentPos.y < BOARD_SIZE_Y))
                            continue;
                        //std::cout << "[Debug] Find available: " << currentPos.x << " , " << currentPos.y << "\n";
                        _availPosRecord.push_back(currentPos);
                        addChess(currentPos.x, currentPos.y, color);
                        setChessStatus(currentPos.x, currentPos.y, Chess::Status::OnBoard, false);
                        setChessStatus(currentPos.x, currentPos.y, Chess::Status::Hint, true);
                        isEnd = false;
                    }
                }

            }
        }
    }
}

void BoardManager::removeChess(const int& x, const int& y)
{
    if (0 <= x && x < BOARD_SIZE_X && 0 <= y && y < BOARD_SIZE_Y && this->_board[y][x]) {
        Chess* tmpChess = this->_board[y][x];
        this->_board[y][x] = nullptr;
        delete tmpChess;
        //std::cout << "[Debug] Remove Chess in: (" << x << " , " << y << ")\n";
    }
    else {
        //std::cout << "[Debug] Remove Chess Failed \n";
    }
}

void BoardManager::checkFliping(const int& x, const int& y)
{
    if (!this->_board[y][x]) {
        return;
    }

    Chess* chess = this->_board[y][x];
    Chess::Color color = chess->getColor();

    for (int dirIdx = 0; dirIdx <= 7; dirIdx++) {

        glm::ivec2 dir = { DirTable[dirIdx][0], DirTable[dirIdx][1] };
        //std::cout << "[Debug] Checking in direction: " << dir.x << " , " << dir.y << "\n";

        glm::ivec2 currentPos = { x, y };
        currentPos += dir;

        if (!(0 <= currentPos.x && currentPos.x < BOARD_SIZE_X && 0 <= currentPos.y && currentPos.y < BOARD_SIZE_Y)) {
            continue;
        }

        bool CanFlip = false;

        int step = 0;

        while (0 <= currentPos.x && currentPos.x < BOARD_SIZE_X &&
            0 <= currentPos.y && currentPos.y < BOARD_SIZE_Y &&
            this->_board[currentPos.y][currentPos.x]) {

            Chess* tmpChess = getChess(currentPos.x, currentPos.y);

            if (!tmpChess) {
                break;
            }

            if (tmpChess->getColor() == color) {
                CanFlip = true;
                break;
            }
            else {
                step++;
            }

            currentPos += dir;
        }

        while (CanFlip && step--) {
            currentPos -= dir;
            Chess* tmpChess = getChess(currentPos.x, currentPos.y);

            if (!tmpChess) {
                break;
            }
            tmpChess->flipColor();
        }
    }
}

void BoardManager::setGameStart()
{
    gameStart = true;
}

void BoardManager::clearChess()
{
    for (int i = 0; i < BOARD_SIZE_Y; i++) {
        for (int j = 0; j < BOARD_SIZE_X; j++) {
            removeChess(j, i);
        }
    }

}

void BoardManager::clearHintChess()
{
    for (const auto& idx : _availPosRecord) {
        removeChess(idx.x, idx.y);
    }
    _availPosRecord = vector<glm::ivec2>();
}

Chess* BoardManager::getHoverChess()
{
    for (const auto& idx : _availPosRecord) {
        if (_board[idx.y][idx.x]->getStatus(Chess::Status::hover)) {
            return _board[idx.y][idx.x];
        }
    }
    return nullptr;
}

bool BoardManager::isFull()
{
    for (int i = 0; i < BOARD_SIZE_Y; i++) {
        for (int j = 0; j < BOARD_SIZE_X; j++) {
            if (_board[j][i]) return true;
        }
    }
    return false;
}

Chess::Color BoardManager::setWinner()
{
    int blackNum = 0;
    int whiteNum = 0;

    for (int i = 0; i < BOARD_SIZE_Y; i++) {
        for (int j = 0; j < BOARD_SIZE_X; j++) {
            if (_board[j][i] && _board[j][i]->getColor() == Chess::Color::Black) {
                blackNum++;
            }
            else {
                whiteNum++;
            }
            
        }
    }
    if(blackNum > whiteNum)
        return Chess::Color::Black;

    return Chess::Color::White;
}

BoardManager::GameStatus BoardManager::getGameStatus()
{
    return _gameStatus;
}

Chess* BoardManager::getChess(const int& x, const int& y)
{
    if (!(0 <= x && x < BOARD_SIZE_X && 0 <= y && y < BOARD_SIZE_Y))
        return nullptr;

    return _board[y][x];
}

Chess::Color BoardManager::getWinner()
{
    return winnwer;
}

void BoardManager::botPlay(glm::ivec2& selectedId)
{
    glm::ivec2 bestPos = glm::ivec2(-1,-1);
    for (const auto& val : _availPosRecord) {
        if (bestPos == glm::ivec2(-1, -1)) {
            bestPos = val;
            continue;
        }

        if (posWeight(bestPos.x, bestPos.y) < posWeight(val.x, val.y)) {
            bestPos = val;
        }
    }
    selectedId = bestPos;
}

int BoardManager::posWeight(int x)
{
    int score = 0;
    if (x == 1 || x == 6) {
        score += 0;
    }
    else if (x == 2 || x == 5) {
        score += 1;
    }
    else if (x == 4 || x == 3) {
        score += 2;
    }

    return score;
}

int BoardManager::posWeight(int x, int y)
{
    return posScoreTable[y][x];
}

Chess* BoardManager::newChess(const Chess::Color& color)
{
    std::cout << "[Debug] newChess: " << int(color) << "\n";
    Chess* c = new Chess(color, blackModel, whiteModel);
    return c;
}

void BoardManager::update(updateContent content)
{
    static float aiSleep = 0;

    static glm::ivec2 selectedIdx;
    bool hit = false;
    for (const auto& idx : _availPosRecord) {
        Transform tf = _board[idx.y][idx.x]->m_transform;
        tf.scale = glm::vec3(1.5, 1.0, 1.5);
        bool hover = _board[idx.y][idx.x]->bb.raycast(content.camera->Position, content.camera->Front, 1000, tf.getModelMatrix());
        _board[idx.y][idx.x]->setStatus(Chess::Status::hover, hover);
    }
    if (_rightMouseState == MouseState::Clicked) {
        for (const auto& idx : _availPosRecord) {
            if (_board[idx.y][idx.x]->getStatus(Chess::Status::hover)) {
                hit = true;
                selectedIdx = idx;
                break;
            }
        }
        _rightMouseState = MouseState::Idle;
    }

    switch (_gameStatus)
    {
    case BoardManager::GameStatus::Preparing:
        if (gameStart) {
            addChess(3, 3, Chess::Color::White);
            addChess(4, 4, Chess::Color::White);
            addChess(3, 4, Chess::Color::Black);
            addChess(4, 3, Chess::Color::Black);

            _gameStatus = GameStatus::SetBlackHint;
            _rightMouseState = MouseState::Idle;
        }
        break;
    case BoardManager::GameStatus::SetBlackHint:
        std::cout << "[Debug] GameStatus::SetBlackHint\n";
        std::cout << "[Debug] Find Available Position: Black\n";
        findAvailablePos(Chess::Color::Black);
        _gameStatus = BoardManager::GameStatus::WaitForBlack;
        std::cout << "[Debug] GameStatus::WaitForBlack\n";
        break;
    case BoardManager::GameStatus::WaitForBlack:
        // end
        if (!_availPosRecord.size()) {
            blackOutOfMove = true;
            if ((blackOutOfMove && whiteOutOfMove)) {
                _gameStatus = BoardManager::GameStatus::End;
                std::cout << "[Debug] GameStatus::End\n";
            }
            else {
                _gameStatus = BoardManager::GameStatus::SetWhiteHint;
                clearHintChess();
            }
            return;
        }
        else {
            blackOutOfMove = false;
        }

        if (BlackBot) {
            if (aiSleep >= aiSpeed) {
                botPlay(selectedIdx);
                std::cout << "[Debug] Hit " << selectedIdx.x << " , " << selectedIdx.y << " !\n";
                _gameStatus = BoardManager::GameStatus::BlackSeletced;
                std::cout << "[Debug] GameStatus::BlackSeletced\n";
                aiSleep = 0.0f;
            }
            else {
                aiSleep += content.deltaTime;
            }
        }

        if (hit && !BlackBot) {
            std::cout << "[Debug] Hit " << selectedIdx.x << " , " << selectedIdx.y << " !\n";
            _gameStatus = BoardManager::GameStatus::BlackSeletced;
            std::cout << "[Debug] GameStatus::BlackSeletced\n";

        }
        break;
    case BoardManager::GameStatus::BlackSeletced:
        if (!_board[selectedIdx.y][selectedIdx.x]->getStatus(Chess::Status::Hint)) {
            std::cout << "[Debug] Selected chess can't move" << "\n";
            _gameStatus = BoardManager::GameStatus::WaitForBlack;
            std::cout << "[Debug] GameStatus::WaitForBlack\n";
        }
        else {
            clearHintChess();
            addChess(selectedIdx.x, selectedIdx.y, Chess::Color::Black);
            checkFliping(selectedIdx.x, selectedIdx.y);
            _gameStatus = BoardManager::GameStatus::SetWhiteHint;
            std::cout << "[Debug] GameStatus::SetWhiteHint\n";
        }
        break;
    case BoardManager::GameStatus::SetWhiteHint:
        std::cout << "[Debug] GameStatus::SetWhiteHint\n";
        std::cout << "[Debug] Find Available Position: White\n";
        findAvailablePos(Chess::Color::White);
        _gameStatus = BoardManager::GameStatus::WaitForWhite;
        std::cout << "[Debug] GameStatus::WaitForWhite\n";
        break;
    case BoardManager::GameStatus::WaitForWhite:
        if (!_availPosRecord.size()) {
            whiteOutOfMove = true;
            if ((blackOutOfMove && whiteOutOfMove)) {
                _gameStatus = BoardManager::GameStatus::End;
                std::cout << "[Debug] GameStatus::End\n";
            }
            else {
                _gameStatus = BoardManager::GameStatus::SetBlackHint;
                clearHintChess();
            }
            return;
        }
        else {
            whiteOutOfMove = false;
        }

        if (WhiteBot) {
            if (aiSleep >= aiSpeed) {
                botPlay(selectedIdx);
                std::cout << "[Debug] Hit " << selectedIdx.x << " , " << selectedIdx.y << " !\n";
                _gameStatus = BoardManager::GameStatus::WhiteSeletced;
                std::cout << "[Debug] GameStatus::WhiteSeletced\n";
                aiSleep = 0.0f;
            }
            else {
                aiSleep += content.deltaTime;
            }
        }

        if (hit && !WhiteBot) {
            std::cout << "[Debug] Hit " << selectedIdx.x << " , " << selectedIdx.y << " !\n";
            _gameStatus = BoardManager::GameStatus::WhiteSeletced;
            std::cout << "[Debug] GameStatus::WhiteSeletced\n";
        }
        break;
    case BoardManager::GameStatus::WhiteSeletced:
        if (!_board[selectedIdx.y][selectedIdx.x]->getStatus(Chess::Status::Hint)) {
            std::cout << "[Debug] Selected chess can't move" << "\n";
            _gameStatus = BoardManager::GameStatus::WaitForWhite;
            std::cout << "[Debug] GameStatus::WaitForWhite\n";
        }
        else {
            clearHintChess();
            addChess(selectedIdx.x, selectedIdx.y, Chess::Color::White);
            checkFliping(selectedIdx.x, selectedIdx.y);
            _gameStatus = BoardManager::GameStatus::SetBlackHint;
            std::cout << "[Debug] GameStatus::SetBlackHint\n";
        }
        break;
    case BoardManager::GameStatus::End:
        winnwer = setWinner();
        clearHintChess();
        gameStart = false;
        break;
    default:
        break;
    }

    for (const auto& row : this->_board) {
        for (const auto& chess : row) {
            if (chess) {
                chess->update(content.deltaTime);
            }
        }
    }
}

void BoardManager::draw(Shader* shader)
{
    for (const auto& row : this->_board) {
        for (const auto& chess : row) {
            if (chess) {
                chess->draw(shader);
            }
        }
    }
}

void BoardManager::processInput(GLFWwindow* window)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        _rightMouseState = MouseState::Clicked;
    }
}
