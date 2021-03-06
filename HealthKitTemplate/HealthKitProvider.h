//
//  HealthKitProvider.h
//  HealthKitTemplate
//
//  Created by Sense Health on 16/06/16.
//  Copyright © 2016 SenseHealth. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <HealthKit/Healthkit.h>

@interface HealthKitProvider : NSObject

+ (HealthKitProvider*)sharedInstance;

@property (nonatomic, retain) HKHealthStore *healthStore;

# pragma mark - Healthkit Permissions

/**
 *
 *   This method asks the user for the permissions to access Healthkit data. In this case, we ask for the
 *   permissions of the different data types at once. So the user can enable all of them at the same time. A part from that, with this method
 *   we ask for to read data that already exists in Healthkit and also to write new data to Healthkit.
 *
 *   In this example, we ask permissions for: Walking and Running, Cycling, Step counter and Sleep Analysis.
 *
 **/

- (void) requestHealthKitAuthorizationForDataTypes:(NSArray *)dataTypes withCompletion:(void(^)(BOOL success, NSError *error))completion;

- (void) requestHealthKitAuthorization:(void(^)(BOOL success, NSError *error))completion;

/**
 *
 *   Similar as the previous method, but here we have a dataType string as an input.
 *   So in this method we ask permissions for only one dataType. In this case, we ask for permissions for a QuantityType.
 *   In this examples, our quantityTypes are stepCounter, walking and running, and cycling.
 *
 **/
- (void) requestHealthKitAuthorizationForHKDataQuantityType:(NSString*)dataType withCompletion:(void(^)(BOOL success, NSError *error))completion;

/**
 *
 *   requestHealthKitAuthorizationForHKDataCategoryType: In this case, we ask for permissions for a QuantityType.
 *   In this examples, we have only one category type, sleep analysis.
 *
 **/
- (void) requestHealthKitAuthorizationForHKDataCategoryType:(NSString*)dataType withCompletion:(void(^)(BOOL success, NSError *error))completion;


#pragma mark - Reading data from Healthkit

/**
 *   It reads the cumulative steps between two given dates.
 *
 **/
- (void) readCumulativeStepsFrom:(NSDate *)startDate toDate:(NSDate *)endDate withCompletion:(void (^)(int steps, NSError *error))completion;

/**
 *
 *   Using the dates that a stepCount dataPoint has, this method calculates an aproximation of the time that the user has been active between two different dates. 
 *   It uses a simple algorithm, with two simple rules: 1. If the stepCount value is lower than 45 steps, it discards it. 2. If the difference between two different 
 *   dataPoints is higher than 200 seconds, it considers that the user has not been moving between this time. (While moving, healhtkit sends dataPoints at least every two minutes.
 *
 **/
- (void) readStepsTimeActiveFromDate:(NSDate*)startDate toDate:(NSDate*)endDate withCompletion:(void (^)(NSTimeInterval timeInterval, NSError *error))completion;


//reading walking and running
- (void) readWalkingTimeActiveFromDate:(NSDate*) startDate toDate:(NSDate*) endDate withCompletion:(void (^)(NSTimeInterval timeActive, NSError *error))completion;

- (void) readCoveredWalkingDistanceFromDate:(NSDate *)startDate toDate:(NSDate*)endDate withCompletion:(void (^)(double totalDistance, NSArray * listOfSpeed, NSError *error)) completion;


//reading cycling
- (void) readCyclingTimeActiveFromDate:(NSDate*) startDate toDate:(NSDate*) endDate withCompletion:(void (^)(NSTimeInterval timeActive, NSError *error))completion;


- (void) readCoveredCyclingDistanceFromDate:(NSDate *)startDate toDate:(NSDate*)endDate withCompletion:(void (^)(double totalDistance, NSArray * listOfSpeed, NSError *error)) completion;


//reading sleep time
- (void) readSleepAnalysisFromDate:(NSDate *)startDate toDate:(NSDate *) endDate withCompletion:(void (^)(NSTimeInterval sleepTime, NSError *error)) completion;


#pragma mark - Writing data to Healthkit

- (void) writeWalkingRunningDistance:(double)distance fromStartDate:(NSDate*)startDate toEndDate:(NSDate*)endDate withCompletion:(void (^)(bool savedSuccessfully, NSError *error))completion;

- (void) writeCyclingDistance:(double)distance fromStartDate:(NSDate*) startDate toEndDate:(NSDate*) endDate withCompletion:(void (^)(bool savedSuccessfully, NSError *error))completion;

- (void) writeSteps:(double)steps fromStartDate:(NSDate*) startDate toEndDate:(NSDate*) endDate withCompletion:(void (^)(bool savedSuccessfully, NSError *error))completion;

- (void) writeSleepAnalysisFromStartDate:(NSDate*)startDate toEndDate:(NSDate*)endDate withCompletion:(void (^)(bool savedSuccessfully, NSError *error))completion;


#pragma mark - HKSource 

- (void) checkSourcesFromStartDate:(NSDate *)startDate toDate:(NSDate *)endDate;



@end
