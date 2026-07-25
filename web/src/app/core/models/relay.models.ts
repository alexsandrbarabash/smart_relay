export type RelayState = 'on' | 'off' | 'scheduled';
export type RelayTab = 'power' | 'timer' | 'schedule';
export type TimerUnit = 'minutes' | 'hours';

export interface RelayStatus {
  deviceName: string;
  relayState: RelayState;
  physicalState: boolean;
  telegramActive: boolean;
  time: number;
}

export interface ScheduleRelayCommand {
  operation: 1 | 0;
  time: number;
}
