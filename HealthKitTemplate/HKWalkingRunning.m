//
//  HKWalkingRunning.m
//  HealthKitTemplate
//
//  Created by Xavier Ramos Oliver on 22/06/16.
//  Copyright © 2016 SenseHealth. All rights reserved.
//

#import "HKWalkingRunning.h"

@implementation HKWalkingRunning

- (void) readWalkingTimeActiveFromStartDate:(NSDate*) startDate toEndDate:(NSDate*) endDate withCompletion:(void (^)(NSTimeInterval timeActive, NSError *error))completion{
    
    HKSampleType *walkingSample = [HKSampleType quantityTypeForIdentifier:HKQuantityTypeIdentifierDistanceWalkingRunning];
    NSPredicate *predicate = [HKQuery predicateForSamplesWithStartDate:startDate endDate:endDate options:HKQueryOptionNone];
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:HKSampleSortIdentifierStartDate ascending:NO];
    
    HKSampleQuery *query = [[HKSampleQuery alloc] initWithSampleType:walkingSample
                                                           predicate:predicate
                                                               limit:HKObjectQueryNoLimit
                                                     sortDescriptors:@[sortDescriptor]
                                                      resultsHandler:^(HKSampleQuery * _Nonnull query, NSArray<__kindof HKSample *> * _Nullable results, NSError * _Nullable error) {
                                                          NSTimeInterval timeActive = 0;
                                                          for (HKQuantitySample *sample in results) {
                                                              if ([sample.quantity doubleValueForUnit:[HKUnit meterUnit]] >= 1){
                                                                  timeActive += [sample.endDate timeIntervalSinceDate:sample.startDate];
                                                              }
                                                          }
                                                          completion(timeActive,error);
                                                      }];
    [[HealthKitProvider sharedInstance].healthStore executeQuery:query];
}

- (void) readMostRecentWalkingTimeActiveSampleWithCompletion:(void (^)(NSTimeInterval timeActive, NSError *error))completion{
    HKSampleType *walkingSample = [HKSampleType quantityTypeForIdentifier:HKQuantityTypeIdentifierDistanceWalkingRunning];
    //TODO: what happens when a user adds a data point with a past startDate? HealthKit tell us that there is a change and we will retrieve the more newer startDate sample and not the most recent added.
    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:HKSampleSortIdentifierStartDate ascending:NO];
    
    HKSampleQuery *query = [[HKSampleQuery alloc] initWithSampleType:walkingSample
                                                           predicate:nil
                                                               limit:1
                                                     sortDescriptors:@[sortDescriptor]
                                                      resultsHandler:^(HKSampleQuery * _Nonnull query, NSArray<__kindof HKSample *> * _Nullable results, NSError * _Nullable error) {
                                                          NSTimeInterval timeActive = 0;
                                                          HKQuantitySample *sample = results[0];
                                                          if ([sample.endDate isEqualToDate:sample.startDate]) {
                                                              //In case of a manually added sample: (when startDate is equal as endDate)
                                                              double distance = [[sample quantity] doubleValueForUnit:[HKUnit meterUnitWithMetricPrefix:HKMetricPrefixKilo]];
                                                              double averageWalkingSpeed = 5.0; //5.0 km/h
                                                              timeActive = distance/averageWalkingSpeed * 3600;
                                                          }else{
                                                              timeActive += [sample.endDate timeIntervalSinceDate:sample.startDate];
                                                          }
                                                          completion(timeActive,error);
                                                      }];
    [[HealthKitProvider sharedInstance].healthStore executeQuery:query];
}

- (void) setTimeActiveOnBackgroundForWalkingSample{
    
    HKSampleType *walkingSample = [HKSampleType quantityTypeForIdentifier:HKQuantityTypeIdentifierDistanceWalkingRunning];
    HKObserverQuery *query = [[HKObserverQuery alloc] initWithSampleType:walkingSample predicate:nil updateHandler:^(HKObserverQuery *query, HKObserverQueryCompletionHandler completionHandler, NSError *error) {
        if (error) {
            NSLog(@"*** An error occured while setting up the stepCount observer. %@ ***",
                  error.localizedDescription);
            abort();
        }
        [self readMostRecentWalkingTimeActiveSampleWithCompletion:^(NSTimeInterval timeActive, NSError *error) {
            NSLog(@"Added walking dataPoint: %.2f seconds", timeActive);
        }];
    }];
    
    [[HealthKitProvider sharedInstance].healthStore executeQuery:query];
    [[HealthKitProvider sharedInstance].healthStore enableBackgroundDeliveryForType:walkingSample frequency:HKUpdateFrequencyImmediate withCompletion:^(BOOL success, NSError *error){
        if (success) {
            NSLog(@"success background changes");
        }
    }];
}

@end
