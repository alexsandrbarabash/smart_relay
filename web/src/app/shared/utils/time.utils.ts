import { TimerUnit } from '../../core/models/relay.models';

const pad = (value: number) => value.toString().padStart(2, '0');

export function toUnixSeconds(date: Date): number {
  return Math.floor(date.getTime() / 1000);
}

export function formatUnixSeconds(timestamp: number): string {
  if (!Number.isFinite(timestamp) || timestamp <= 0) {
    return '----.--.-- --:--:--';
  }

  const date = new Date(timestamp * 1000);
  const year = date.getFullYear();
  const month = pad(date.getMonth() + 1);
  const day = pad(date.getDate());
  const hours = pad(date.getHours());
  const minutes = pad(date.getMinutes());
  const seconds = pad(date.getSeconds());

  return `${year}.${month}.${day} ${hours}:${minutes}:${seconds}`;
}

export function addDurationToNow(amount: number, unit: TimerUnit): number {
  const multiplier = unit === 'hours' ? 60 * 60 * 1000 : 60 * 1000;
  return toUnixSeconds(new Date(Date.now() + amount * multiplier));
}

export function localDateTimeToUnixSeconds(date: string, time: string): number {
  return toUnixSeconds(new Date(`${date}T${time}`));
}

export function todayDateInput(): string {
  const now = new Date();
  return `${now.getFullYear()}-${pad(now.getMonth() + 1)}-${pad(now.getDate())}`;
}

export function nextHourTimeInput(): string {
  const nextHour = new Date(Date.now() + 60 * 60 * 1000);
  return `${pad(nextHour.getHours())}:${pad(nextHour.getMinutes())}`;
}
