# Project JA (Action RPG)

> **Dedicated Server** 기반의 고도화된 **Gameplay Ability System (GAS)** 설계 및 실무 프레임워크 구축 프로젝트  
> **Hybrid C++ Architecture**를 통한 성능 최적화와 네트워크 무결성 확보 지향  
> 엔진 버전: **Unreal Engine 5.5.4**

---

## Overview
**Project JA**는 단순 기능 구현을 넘어, 대규모 액션 RPG의 코어 시스템을 **Dedicated Server** 환경에서 설계하고 검증하는 엔지니어링 중심 포트폴리오입니다.  
강의의 베이스라인을 기반으로 하되, 핵심 로직을 **BP -> C++로 리팩토링**하고 **네트워크 예측(Prediction)** 및 **동기화 로직**을 직접 구현하여 실무 수준의 아키텍처 역량을 증명합니다.

---

## Focus & Goals
- **Server-Authoritative GAS 설계** → Dedicated Server 환경에서 Ability, Effect, Attribute의 무결성 보장 및 **GAS Prediction**을 통한 레이턴시 보정  
- **Hybrid C++ 리팩토링 및 최적화** → 블루프린트 로직의 C++ 이식을 통한 유지보수성 향상 및 **Data-Driven(DataTable/DataAsset)** 설계 패턴 확립  
- **심화 시스템 통합 및 확장** → **Custom Climbing(CharacterMovement 확장)**, **MVVM UI**, **멀티플레이어 동기화** 등 복합 시스템의 유기적 통합  

---

## Technical Stack
| 항목 | 내용 |
|------|------|
| **Engine** | Unreal Engine 5.5.4 |
| **Language** | C++17 (Core Logic), Blueprint (Content Only) |
| **Network** | Dedicated Server, RPCs, Replication, Prediction Key |
| **Core Systems** | Gameplay Ability System (GAS), MVVM (UI), Enhanced Input |
| **Movement** | Custom CharacterMovement (Climbing), Motion Warping |
| **Profiling** | Unreal Insights, HISM (Optimization) |

---

## Repository Info
이 저장소는 **C++ 아키텍처 설계 및 네트워크 동기화 로직 기록용**입니다.  
프로젝트의 핵심 클래스와 엔지니어링 의사결정 과정을 공개하며, 대용량 에셋 및 빌드 산출물은 포함되지 않습니다.

---

## License / Usage
- 개인 포트폴리오 및 기술 연구 목적으로 공개되었습니다.  
- 참고 강의: [Udemy – Unreal Engine 5 C++ Advanced Action RPG](https://www.udemy.com/course/unreal-engine-5-advanced-action-rpg-korean/)  
- 사용 리소스:
  1. Bossy Enemy Animation Pack — by kampinis, via Epic Games Fab
  2. Paragon Assets — by Epic Games

---

## Author
**Lim Jaehyeok (임재혁)** Game Client Developer  
jehyuk1711@gmail.com
