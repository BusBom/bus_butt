> 정류장 플랫폼(P1~Pn)에 접근 중인 버스 번호들을, 현재 플랫폼 정차 상태에 따라 예상 도착 위치로 매칭하여 안내 시스템(/dev/bus_display)에 전달하는 시스템.
>
<br/>

- 실제로는 YOLO + OCR → `/sequence`에 접근 버스 번호들 기록 (SHM)
- CGI 서버 `/stop-status`로 각 플랫폼(P1~Pn)의 현재 정차 상태 확인
- "끼어들기 불가능", "한 줄"이라는 실제 도로 특성을 반영한 알고리즘 적용

<br/><br/>

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
