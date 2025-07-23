> 정류장 플랫폼(P1~Pn)에 접근 중인 버스 번호들을, 현재 플랫폼 정차 상태에 따라 예상 도착 위치로 매칭하여 안내 시스템(/dev/bus_display)에 전달하는 시스템.
>
<br/>

- 실제로는 YOLO + OCR → `/sequence(변경된 구조도에서는 /busbom_approach)`에 접근 버스 번호들 기록 (SHM)
- CGI 서버 `/stop-status`로 각 플랫폼(P1~Pn)의 현재 정차 상태 확인
- "끼어들기 불가능", "한 줄"이라는 실제 도로 특성을 반영한 알고리즘 적용

<br/><br/>
## 핵심 규칙
1. sequence는 가까운 순서대로 도착 예정 버스를 나타냄<br/>
2. stop_status가 1인 플랫폼은 버스의 통행을 막음<br/>
3. 버스는 정차 중인 플랫폼(1) 앞을 지나가지 못함.
4. 따라서 정차 중인 플랫폼이 하나라도 등장하면 그 뒤는 접근 불가<br/>
5. 비어 있는 플랫폼(0) 중, 막히지 않은 플랫폼에만 앞 순서의 버스를 순차 할당<br/>


<br/><br/>
## 예시
```
sequence     = [271, 2319, 273, 710]; 
stop_status  = [0, 1, 1, 0];  // P1, P2, P3, P4
```

### 처리 흐름
P1은 비었지만 → 뒤에 P2, P3가 막혀 있으므로 접근 불가

P2, P3는 정차 중이므로 통행 차단

P4는 비었지만 P1~P3가 막혀 있어서 도달 가능

→ 결과: P4 ← 271 만 배정됨
→ 나머지 버스는 할당 불가

<br/><br/><br/>

## `main.cpp`

- 전체 반복 루프를 관리
- 1초마다 sequence + stop-status를 읽고 → 매칭 → 결과 출력
  
<br/><br/>

## `shm_reader.cpp / .h`

- SHM(`/sequence`)에서 버스 번호 벡터를 읽는 함수 구현
```std::vector<int> readSequenceFromSHM();```

<br/><br/>

## `mock_status.cpp / .h`

- CGI 연동 전까지 사용할 임시 stop-status 벡터 반환
```std::vector<int> readStopStatusMock();  // ex: {0, 1, 1, 0}```

<br/><br/>

## `display_writer.cpp / .h`

- 지금은 콘솔에 출력 ( `/dev/bus_display`로 수정 가능)
```void printResultToStdout(const std::vector<std::pair<int, int>>& result);```

<br/><br/>

- `readSequenceFromSHM()` → SHM `/sequence`에서 버스 번호 읽기
- `readStopStatusMock()` → 플랫폼 상태 임시 반환
- `matchBusToPlatforms()` → 알고리즘 수행, 플랫폼별 버스 배정 결과 생성
- `printResultToStdout()` → 결과 콘솔 출력
