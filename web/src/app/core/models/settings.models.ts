export type WifiSetupMode = 'ap' | 'station';

export interface WifiSettingsCommand {
  mode: WifiSetupMode;
  ssid?: string;
  pass?: string;
}

export interface TelegramTokenCommand {
  token: string;
}
