# CLI-Tetris (Terminal Tetris)

Ubuntu, Shell에서 동작하는 1인용 테트리스입니다. 

### Purpose
1. 기능의 추가와 유지보수가 용의한 소프트웨어 개발
2. 객체지향 설계와 Design Pattern
3. 효율적인 협업을 위한 방법
4. C++ 17 적용


### Environment
* Ubuntu 16.04
* C++ 17
* GCC
* VS Code
* WSL (Windows Subsystem for Linux)
* ncurse 5.0


### Design
전체적인 설계는 다음의 목적에 맞게 클래스가 존재합니다.

* GameManager (게임의 실행, 상태 변경 책임)
* GameState (플레이에 대한 상태를 표현할 책임)
* UiHandler (출력할 책임)
* TimerHandler (Timer 관리 책임)
* RandomValueHandler (Ramdom value를 만드는 책임)
* GraphicObject (출력할 내용을 보유할 책임)
* Locator (각 Handler를 관리하고, 핸들러의 유일성을 보장할 책임)
* GameEngine (테트리스의 본연의 기능을 책임)
* TimerAccessor (Timer의 수명 및 상태를 보여줄 책임)


#### GameManager와 GameState의 관계
![GameManager-GameState](https://user-images.githubusercontent.com/62828810/166137004-87d3b002-3647-4bbf-8ea1-df2ab1a05b0c.svg)

#### GraphicObject와 UiHandler의 관계
![GraphicObject-UiHandler drawio](https://user-images.githubusercontent.com/62828810/166137014-3fe904e4-c54f-4cc4-9721-97d8f5aad91f.svg)

#### TimerHandler와 TimerAccessor 의 관계
![TimerHandler-TimerAccessor drawio](https://user-images.githubusercontent.com/62828810/166137028-a5b0881f-020e-4250-8737-211925eff984.svg)


### Issue
1. 터미널 화면 크기 조정 불가능
2. C++ chrono를 짧은 단위로 호출할시, 정상적인 시간출력이 어려운 문제
3. ncurse는 C Library이기 떄문에 발생하는 Wrap Class 소요
4. ncurse library color display는 Thread safe하지 않음

### 협업을 위해 고민하고 적용했던 점
- Google C++ Style
- 일관된 Commit 방식 정의
 


### 실행 화면
![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/0bb3e133-a97f-403a-86d7-32e1ef065fbb/Untitled.png)


### 느낀 점 (회고)
처음부터 완벽하게 만들 계획을 가지진 않았지만 테트리스의 기본 기능만 구현하고 중단하기로 결정했습니다. 

- 점수 추가
- 레벨에 따른 블록 가속
- Inform 내용 출력
- 다음 블럭 출력
위와 같이 필요하다고 생각해둔 내용이 있지만, Framework가 없는 상태에서 다음의 기능들을 구현하려면 더 많은 구조들을 스스로 도입해야한다는 점이 압박으로 느껴졌기 때문입니다. 구체적으로는 아래와 같은 협력이 더 필요합니다.

- UserData를 구현하고 내용을 저장하는 객체
- 발생한 이벤트를 처리할 객체와 GameState 간의 협력 방법
- 상응하는 GraphicObject 추가

특히 GraphicObject를 만드는 것 자체가 굉장한 압박감으로 다가왔습니다. 보기 좋은 프레임워크가 없기 때문에 좌표값을 계산해서 Object를 만드는 일은 본래 프로젝트 목적과는 다르게 더 많은 로드를 발생시켰습니다. 디버그 환경조차 계속 다시 실행해서 확인하는 방법 뿐이였습니다. 따라서 더 진행하는 것이 의미가 없지는 않지만 직장을 다니면서 준비하기에는 어렵다고 판단했고 추후에 다른 플랫폼에서 진행하기로 결정했습니다.

그리고 다음과 같은 점에서 고민해보게 됬습니다. 앞으로 고민해봐야할 과제가 될 것이라 생각합니다.
- 용이한 디버그 환경
- Unit Test (자동화된 Test)


### if you intrest ...
만약 지금 프로젝트에 관심이 있다면 연락하셔도 좋습니다. ncurse의 사용법에 대해서도 참고하셔도 좋습니다.

ncurse의 자세한 사용법은 공식 홈페이지를 참고했습니다.
https://invisible-island.net/ncurses/man/ncurses.3x.html#h3-COLUMNS

